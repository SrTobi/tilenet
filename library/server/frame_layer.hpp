#pragma once
#ifndef _FRAME_LAYER_HPP
#define _FRAME_LAYER_HPP

#include <mutex>
#include <boost/asio/strand.hpp>

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
	class UpdateFrameLayerJob;
}

class FrameLayer
	: public Layer
{
	friend class job::UpdateFrameLayerJob;
public:
	FrameLayer(TNFLAG flags);
	~FrameLayer();

	void makeInitialCommit();
	//void update(TNTILE* tiles, TNBOOL* toupdate);


	OVERRIDE void destroy();
	OVERRIDE shared_ptr<TilenetObject> clone();

private:
	//Commit update(const std::vector<>& tiles, const std::vector<bool>& toupdate);
	Commit makeFullSnapshotCommit(bool asNewCommit);


	virtual OVERRIDE std::vector<Commit> getCommitsUpTo(TNID nr);
	virtual OVERRIDE Commit getDelta( TNID nr );


private:
	boost::asio::strand mUpdateStrand;
	std::mutex			mMutex;
	CommitQueue			mCommits;
};



}





#endif