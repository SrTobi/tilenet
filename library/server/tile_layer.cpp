#include "includes.hpp"

#include "tile_layer.hpp"
#include "layer.hpp"

#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "server/service.hpp"
#include "server/jobs/update_layer_job.hpp"

namespace srv {


TileLayer::TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags)
	: mTileField(size)
	, mRatio(ratio)
	, mUpdateStrand(Service::Inst())
	, mCommits(1)
{
	//makeFullSnapshotCommit(true); // <- falsche id!!!
}

TileLayer::~TileLayer()
{
}


void TileLayer::makeInitialCommit()
{
	makeFullSnapshotCommit(true);
}



void TileLayer::update( TNTILE* tiles, TNBOOL* toupdate )
{
	const unsigned int array_sizes =  mTileField.storage().size();

	std::vector<net::PTile> tileList(tiles, tiles + array_sizes);
	std::vector<bool> updateList(toupdate, toupdate  + (toupdate? array_sizes : 0));

	auto job = std::make_shared<job::UpdateLayerJob>(self<TileLayer>(), std::move(tileList), std::move(updateList));
	Service::Inst().enqueJob(job);
}


Layer::Commit TileLayer::update( const std::vector<net::PTile>& tiles, const std::vector<bool>& toupdate )
{
	// vector for tiles and their offsets later used in the message
	typedef proto::curv::to_client::LayerControl_SendLayerUpdate::update_tile update_tile;
	std::vector<update_tile> update_tiles;
	
	{
		std::lock_guard<std::mutex> guard(mMutex);


		bool has_update_list = toupdate.size() > 0;
		tnAssert(!has_update_list || tiles.size() == toupdate.size());

		auto& storage = mTileField.storage();
		int size = storage.size();
		int last_updated = -1;
		int num_updates = 0;

		for(int i = 0; i < size; ++i)
		{
			if(!has_update_list || (has_update_list && toupdate[i]))
			{
				if(tiles[i] != storage[i])
				{
					assert(i > last_updated || i == 0);
					storage[i] = tiles[i];

					int skiped = i - last_updated - 1;
					tnAssert(i >= 0);

					update_tiles.emplace_back(skiped, tiles[i]);
					++num_updates;
					last_updated = i;
				}
			}
		}
	}

	proto::curv::to_client::LayerControl_SendLayerUpdate patch;

	TNID commitnr = newCommit();
	patch.commitNr = commitnr;
	patch.layerId = id();
	patch.layerContent = std::move(update_tiles);


	Commit com = net::make_message(patch);

	// commit
	mCommits.commitDelta(commitnr, com);

	return com;
}

TileLayer::Commit TileLayer::makeFullSnapshotCommit(bool asNewCommit)
{
	std::lock_guard<std::mutex> guard(mMutex);
	
	TNID commitnr = asNewCommit? newCommit() : currentCommitNr();

	proto::curv::to_client::LayerControl_SendFullLayer fullLayer;

	fullLayer.layerId = id();
	fullLayer.xratio = ratio().x;
	fullLayer.yratio = ratio().y;
	fullLayer.width = size().w;
	fullLayer.height = size().h;
	fullLayer.commitNr = commitnr;

	auto& storage = mTileField.storage();
	auto& content = fullLayer.layerContent;
	content = storage;

	tnAssert(content.size() == size().area());
	tnAssert(storage.size() == content.size());

	Commit com = net::make_message(fullLayer);

	// commit
	mCommits.commit(commitnr, com);

	return com;
}



const Ratio& TileLayer::ratio() const
{
	return mRatio;
}

const Rect& TileLayer::size() const
{
	return mTileField.size();
}

OVERRIDE void TileLayer::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> TileLayer::clone()
{
	NOT_IMPLEMENTED();
}

OVERRIDE std::vector<TileLayer::Commit> TileLayer::getCommitsUpTo( TNID nr )
{
	return std::move(mCommits.getCommitsUpTo(nr));
}

OVERRIDE TileLayer::Commit TileLayer::getDelta( TNID nr )
{
	return mCommits.getDelta(nr);
}



/*
shared_ptr<net::Message> TileLayer::getStateMessage()
{
	proto::curv::to_client::LayerControl_SendFullLayer fullLayer;

	fullLayer.layerId = id();
	fullLayer.xratio = ratio().x;
	fullLayer.yratio = ratio().y;
	fullLayer.width = size().w;
	fullLayer.height = size().h;

	auto& storage = mTileField.storage();
	auto& content = fullLayer.layerContent;
	content.reserve(storage.size());

	for(auto& tile : mTileField.storage())
	{
		
		content.push_back(net::PTile(tile));
	}

	tnAssert(content.size() == size().area());
	tnAssert(storage.size() == content.size());
	

	return net::make_message(fullLayer);
}*/


}