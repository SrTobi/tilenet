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
	clear();
}

TileLayer::~TileLayer()
{
}


void TileLayer::clear()
{
	// Clear the layer
	auto& storage = mTileField.storage();
	memset(storage.data(), 0, storage.size() * sizeof(TNTILE));
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
	std::lock_guard<std::mutex> guard(mMutex);


	// apply changes
	if(toupdate.size())
	{
		auto& storage = mTileField.storage();
		assert(storage.size() == tiles.size());
		assert(storage.size() == toupdate.size());
		
		auto tile_it = tiles.begin();
		auto upd_it = toupdate.begin();
		auto stor_it = storage.begin();

		for(;stor_it != storage.end(); ++tile_it, ++upd_it, ++stor_it)
		{
			if(*upd_it)
				*stor_it = *tile_it;
		}

	}else{
		auto& storage = mTileField.storage();
		assert(storage.size() == tiles.size());
		storage = tiles;
	}

	// build commit
	TNID commitnr = newCommit();

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