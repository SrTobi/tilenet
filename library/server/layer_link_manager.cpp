#include "includes.hpp"
#include "layer_link_manager.hpp"


namespace srv {



LayerLinkManager::LayerLinkManager()
{
}

LayerLinkManager::~LayerLinkManager()
{
}

void LayerLinkManager::linkLayerToParticipant( const shared_ptr<Layer>& layer, const shared_ptr<Participant>& participant )
{
	unlinkParticipant(participant);
	mLinks.emplace_back(layer, participant);
}

void LayerLinkManager::updateFrameLinks( const shared_ptr<Frame>& frame )
{
	NOT_IMPLEMENTED();
}

void LayerLinkManager::unlinkLayer( const shared_ptr<Layer>& layer )
{
	auto it = mLinks.begin();

	while(it != mLinks.end())
	{
		if(it->first == layer)
		{
			mLinks.erase(it++);
		}else{
			it++;
		}
	}
}

void LayerLinkManager::unlinkParticipant( const shared_ptr<Participant>& participant )
{
	for(auto it = mLinks.begin(); it != mLinks.end(); ++it)
	{
		if(it->second == participant)
		{
			mLinks.erase(it);
			return;
		}
	}
}

std::vector<shared_ptr<Participant>> LayerLinkManager::getLinkedParticipants(const shared_ptr<Layer>& layer) const
{
	std::vector<shared_ptr<Participant>> participants;

	for(auto& p : mLinks)
	{
		if(p.first == layer)
		{
			participants.push_back(p.second);
		}
	}

	return std::move(participants);
}

LayerLinkManager& LayerLinkManager::Inst()
{
	static LayerLinkManager inst;
	return inst;
}






}