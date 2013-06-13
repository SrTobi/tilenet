#include "includes.hpp"
#include "update_layer_job.hpp"
#include "server/tile_layer.hpp"
#include "server/layer_link_manager.hpp"
#include "server/participant.hpp"

namespace srv{
namespace job {




UpdateLayerJob::UpdateLayerJob( const shared_ptr<TileLayer>& layer, std::vector<net::PTile>&& tiles, std::vector<bool>&& toupdate)
	: mLayer(layer)
	, mTiles(std::move(tiles))
	, mToUpdate(std::move(toupdate))
{
}

UpdateLayerJob::~UpdateLayerJob()
{
}

OVERRIDE void UpdateLayerJob::process()
{
	auto& llm = LayerLinkManager::Inst();

	auto commit = mLayer->update(mTiles, mToUpdate);

	{
		auto participants = llm.getLinkedParticipants(mLayer);

		for(auto& p : participants)
			p->port()->send(commit);
	}
}



}}