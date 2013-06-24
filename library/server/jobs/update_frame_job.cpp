#include "includes.hpp"
#include "update_frame_job.hpp"
#include "server/frame_layer.hpp"

#include "server/layer_link_manager.hpp"
#include "server/participant.hpp"


namespace srv {
namespace job {
	
UpdateFrameJob::UpdateFrameJob( const shared_ptr<FrameLayer>& frame, std::unordered_map<shared_ptr<Layer>, std::pair<PView, unsigned short>>&& sublayers)
	: mFrame(frame)
	, mSublayers(std::move(sublayers))
{
}


UpdateFrameJob::~UpdateFrameJob()
{
}

OVERRIDE void UpdateFrameJob::process()
{
	auto& llm = LayerLinkManager::Inst();
	auto commit = mFrame->update(std::move(mSublayers));

	{
		auto participants = llm.getLinkedParticipants(mFrame);

		for(auto& p : participants)
			p->port()->send(commit);
	}
}






}}