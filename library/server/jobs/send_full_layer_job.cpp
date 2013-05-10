#include "includes.hpp"
#include "send_full_layer_job.hpp"

#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "server/layer.hpp"
#include "server/participant.hpp"
#include "server/layer_link_manager.hpp"



namespace srv {
namespace job {







SendFullLayerJob::SendFullLayerJob( const shared_ptr<Layer>& layer )
	: mLayer(layer)
{

}

SendFullLayerJob::~SendFullLayerJob()
{

}

OVERRIDE void SendFullLayerJob::process()
{
	LayerLinkManager& llm = LayerLinkManager::Inst();

	auto participants = llm.getLinkedParticipants(mLayer);

	auto msg = mLayer->getStateMessage();

	for(auto p : participants)
	{
		p->port()->send(msg);
	}

}

}}