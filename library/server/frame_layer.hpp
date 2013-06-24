#pragma once
#ifndef _FRAME_LAYER_HPP
#define _FRAME_LAYER_HPP

#include <mutex>
#include <boost/asio/strand.hpp>
#include <unordered_map>

#include "settings.hpp"

#include "layer.hpp"
#include "utils/field.hpp"
#include "commit_queue.hpp"

#include "network/v1.0/protocol_tile.hpp"

namespace net {
	class Message;
}


namespace srv {

namespace job {
	class UpdateFrameJob;
}

class FrameLayer
	: public Layer
{
	typedef proto::curv::PView PView;
	friend class job::UpdateFrameJob;
public:
	FrameLayer(TNFLAG flags);
	~FrameLayer();

	void init();
	void update(TNLAYER* layer_list, TNVIEW** view_list, size_t size);


	OVERRIDE void destroy();
	OVERRIDE shared_ptr<TilenetObject> clone();

private:
	Commit update(std::unordered_map<shared_ptr<Layer>, std::pair<PView, unsigned short>>&& sublayers);
	Commit makeFullSnapshotCommit(bool asNewCommit);
	void makeInitialCommit();


	virtual OVERRIDE std::vector<Commit> getCommitsUpTo(TNID nr);
	virtual OVERRIDE Commit getDelta( TNID nr );


private:
	boost::asio::strand mUpdateStrand;
	std::mutex			mMutex;
	CommitQueue			mCommits;

	std::unordered_map<shared_ptr<Layer>, std::pair<PView, unsigned short>> mSubLayers;
};



}





#endif