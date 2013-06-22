#include "includes.hpp"

#include "frame_layer.hpp"
#include "layer.hpp"

#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "server/service.hpp"
#include "server/jobs/update_frame_job.hpp"
#include "server/layer_link_manager.hpp"

namespace srv {



FrameLayer::FrameLayer( TNFLAG flags )
	: mUpdateStrand(Service::Inst())
	, mCommits(1)
{
}

FrameLayer::~FrameLayer()
{
}

void FrameLayer::makeInitialCommit()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void FrameLayer::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> FrameLayer::clone()
{
	NOT_IMPLEMENTED();
}

FrameLayer::Commit FrameLayer::makeFullSnapshotCommit( bool asNewCommit )
{
	std::lock_guard<std::mutex> guard(mMutex);

	TNID commitnr = asNewCommit? newCommit() : currentCommitNr();

	proto::curv::to_client::LayerControl_SendFrame fullFrame;

	fullFrame.layerId = id();
	fullFrame.commitNr = commitnr;

	NOT_IMPLEMENTED()
	/*auto& storage = mTileField.storage();
	auto& content = fullLayer.layerContent;
	content = storage;

	tnAssert(content.size() == size().area());
	tnAssert(storage.size() == content.size());*/

	Commit com = net::make_message(fullFrame);

	// commit
	mCommits.commit(commitnr, com);

	return com;
}

void FrameLayer::update( TNLAYER* layer_list, TNVIEW** view_list, size_t size )
{
	const bool update_views = (view_list != nullptr);

	std::unordered_map<shared_ptr<Layer>, std::pair<PView, short>> sublayers;

	short z = 0;
	auto* end = layer_list + size;
	for(;layer_list < end; ++layer_list, ++view_list, ++z)
	{
		auto* layer = dynamic_cast<Layer*>(*layer_list);

		if(!layer)
			NOT_IMPLEMENTED();

		IMPLEMENTATION_TODO("Check view values")

		if(update_views && *view_list)
			sublayers.emplace(layer->self<Layer>(), std::make_pair(PView(**view_list), z));
		else
			sublayers.emplace(layer->self<Layer>(), std::make_pair(PView(), z));
	}


	auto job = std::make_shared<job::UpdateFrameJob>(self<FrameLayer>(), std::move(sublayers));
	Service::Inst().enqueJob(job, mUpdateStrand);
}

FrameLayer::Commit FrameLayer::update(std::unordered_map<shared_ptr<Layer>, std::pair<PView, short>>&& sublayers )
{
	auto& llm = LayerLinkManager::Inst();

	auto updateList = std::move(sublayers);
	std::vector<shared_ptr<Layer>> addList;		// will contain layers, which are new in this frame
	std::vector<shared_ptr<Layer>> removeList;	// will contain layers, which need to be unlinked from this frame

	bool always_view_set = true;
	bool need_to_send_zorder = false;			// if this is true, we have to send the list with zordered layers
	std::vector<TNID> layers_in_zorder(sublayers.size(), 0);
	std::vector<std::pair<TNID, PView>> update_views;
	update_views.reserve(sublayers.size());



	std::lock_guard<std::mutex> guard(mMutex);

	for(auto subl_it = mSubLayers.begin(); subl_it != mSubLayers.end();)
	{
		auto update_it = updateList.find(subl_it->first);

		if(update_it == updateList.end())
		{
			// remove it
			need_to_send_zorder = true;
			removeList.push_back(std::move(subl_it->first));
			subl_it = mSubLayers.erase(subl_it);
		}else{
			const TNID id = subl_it->first->id();

			// process z order
			auto& old_z = subl_it->second.second;
			auto& new_z = update_it->second.second;
			if(old_z != new_z)
				need_to_send_zorder = true;

			new_z = old_z;

			tnAssert(layers_in_zorder[new_z] == 0);
			layers_in_zorder[new_z] = id;

			// update p-view
			auto& old_view = subl_it->second.first;
			auto& update_view = update_it->second.first;
			if(!update_view.empty())
			{
				old_view = update_view;
				update_views.emplace_back(id, update_view); // add it to views who need to be send to participants
			}else{
				always_view_set = always_view_set && old_view.empty();
			}

			// updated... erase it from update list
			updateList.erase(update_it);
			++subl_it;
		}

	}

	// updateList only contains layers waiting to be added to mSubLayers
	// so add them
	for(auto& p : updateList)
	{
		// We need to tell the llm
		addList.push_back(p.first);


		// insert it to mSubLayers and check if it is really new to mSubLayers
		bool newInserted;
		std::tie(std::ignore, newInserted) = mSubLayers.emplace(std::move(p));
		tnAssert(newInserted);

		// add it to z-order list
		size_t z = p.second.second;
		tnAssert(z < layers_in_zorder.size() && layers_in_zorder[z] == 0);
		layers_in_zorder[z] = p.first->id();
	}
	
	{
		// update llm
		llm.updateFrameLinks(self<FrameLayer>(), addList, removeList);
	}

	// build message
	TNID commitnr = newCommit();
	proto::curv::to_client::LayerControl_SendFrame sendframe;

	sendframe.layerId = id();
	sendframe.commitNr = commitnr;
	sendframe.update_views = std::move(update_views);

	if(need_to_send_zorder)
		sendframe.sublayers_in_zorder = std::move(layers_in_zorder);

	Commit commit = net::make_message(sendframe);
		
	// check if it is a full commit
	if(need_to_send_zorder && always_view_set)
	{
		// We have a full commit
		mCommits.commit(commitnr, commit);
	}else{
		// We have a delta
		mCommits.commitDelta(commitnr, commit);
	}

	return commit;
}

OVERRIDE std::vector<FrameLayer::Commit> FrameLayer::getCommitsUpTo( TNID nr )
{
	std::vector<Commit> result;
	{
		std::lock_guard<std::mutex> guard(mMutex);
		result = std::move(mCommits.getCommitsUpTo(nr));
	}

	// if result is empty we have to make a new full commit!
	if(result.empty())
	{
		result.push_back(makeFullSnapshotCommit(false));
	}

	return std::move(result);
}

OVERRIDE FrameLayer::Commit FrameLayer::getDelta( TNID nr )
{
	return mCommits.getDelta(nr);
}

}