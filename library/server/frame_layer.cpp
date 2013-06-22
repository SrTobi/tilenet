#include "includes.hpp"

#include "frame_layer.hpp"
#include "layer.hpp"

#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "server/service.hpp"
//#include "server/jobs/update_layer_job.hpp"

namespace srv {



FrameLayer::FrameLayer( TNFLAG flags )
	: mUpdateStrand(Service::Inst())
	, mCommits(1)
{
	NOT_IMPLEMENTED();
}

FrameLayer::~FrameLayer()
{
	NOT_IMPLEMENTED();
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
	NOT_IMPLEMENTED();
}

OVERRIDE std::vector<FrameLayer::Commit> FrameLayer::getCommitsUpTo( TNID nr )
{
	NOT_IMPLEMENTED();
}

OVERRIDE FrameLayer::Commit FrameLayer::getDelta( TNID nr )
{
	NOT_IMPLEMENTED();
}

}