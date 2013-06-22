#pragma once
#ifndef _LAYER_LINK_MANAGER_HPP
#define _LAYER_LINK_MANAGER_HPP


#include <vector>
#include "settings.hpp"



namespace srv {

class Layer;
class FrameLayer;
class Participant;


class LayerLinkManager
{
public:
	LayerLinkManager();
	~LayerLinkManager();

	void linkLayerToParticipant(const shared_ptr<Layer>& layer, const shared_ptr<Participant>& participant);
	void updateFrameLinks(const shared_ptr<FrameLayer>& frame, const std::vector<shared_ptr<Layer>>& addLayers, const std::vector<shared_ptr<Layer>>& removeLayers);
	void unlinkLayer(const shared_ptr<Layer>& layer);
	void unlinkParticipant(const shared_ptr<Participant>& participant);

	std::vector<shared_ptr<Participant>> getLinkedParticipants(const shared_ptr<Layer>& layer) const;

	static LayerLinkManager& Inst();
private:
	// MOCK!!!
	std::vector<std::pair<shared_ptr<Layer>, shared_ptr<Participant>>> mLinks;
};




}



#endif