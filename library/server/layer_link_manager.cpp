#include "includes.hpp"
#include "layer_link_manager.hpp"

#include "frame_layer.hpp"
#include "utils/helper_functions.hpp"

namespace srv {



class LayerLinkManager::Node
{
public:
	Node(const shared_ptr<Layer>& layer)
		: mLayer(layer)
	{
	}

	~Node()
	{
	}

	void addParticipant(const shared_ptr<Participant>& participant)
	{
		mParticipants.push_back(participant);
	}

	void removeParticipant(const shared_ptr<Participant>& participant)
	{
		size_t num = stdx::swap_remove(mParticipants, participant);
		tnAssert(num == 1);
	}

	void addChild(Node* node)
	{
		tnAssert(node);
		mChildNodes.push_back(node);
		node->_addParent(this);
	}

	void removeChild(Node* node)
	{
		tnAssert(node);

		size_t num = stdx::swap_remove(mChildNodes, node);
		tnAssert(num);

		node->_remove_parent(this);
	}

	void unlink()
	{
		for(Node* node : mParentNodes) node->_remove_child(this);
		for(Node* node : mChildNodes) node->_remove_parent(this);
	}

	void addWatchingParticipants(std::vector<shared_ptr<Participant>>& list) const
	{
		std::copy(mParticipants.begin(), mParticipants.end(), std::back_inserter(list));

		for(Node* n : mParentNodes)
			n->addWatchingParticipants(list);
	}


	void addVisibleLayers(std::vector<shared_ptr<Layer>>& list) const
	{
		list.push_back(mLayer);

		for(Node* n : mChildNodes)
			n->addVisibleLayers(list);
	}

	const std::vector<shared_ptr<Participant>>& participants() const
	{
		return mParticipants;
	}
private:
	void _remove_parent(Node* node)
	{
		size_t num = stdx::swap_remove(mParentNodes, node);
		tnAssert(num == 1);
	}

	void _remove_child(Node* node)
	{
		size_t num = stdx::swap_remove(mChildNodes, node);
		tnAssert(num == 1);
	}

	void _addParent(Node* node)
	{
		mParentNodes.push_back(node);
	}

private:
	const shared_ptr<Layer> mLayer;
	std::vector<Node*> mParentNodes;
	std::vector<Node*> mChildNodes;
	std::vector<shared_ptr<Participant>> mParticipants;
};


LayerLinkManager::LayerLinkManager()
{
}

LayerLinkManager::~LayerLinkManager()
{
}

void LayerLinkManager::linkLayerToParticipant( const shared_ptr<Layer>& layer, const shared_ptr<Participant>& participant )
{
	std::lock_guard<std::mutex> guard(mMutex);

	auto it = mParticipantToNodeMapping.find(participant);

	if(it != mParticipantToNodeMapping.end())
	{
		it->second->removeParticipant(participant);
	}else{
		std::tie(it, std::ignore) = mParticipantToNodeMapping.emplace(participant, nullptr);
	}

	Node* lNode = _layer_node(layer);
	lNode->addParticipant(participant);

	it->second = lNode;
}

void LayerLinkManager::updateFrameLinks( const shared_ptr<FrameLayer>& frame, const std::vector<shared_ptr<Layer>>& addLayers, const std::vector<shared_ptr<Layer>>& removeLayers)
{
	std::lock_guard<std::mutex> guard(mMutex);

	Node* fNode = _layer_node(frame);

	for(auto& layer : removeLayers)
	{
		Node* lNode = _layer_node(layer);
		fNode->removeChild(lNode);
	}

	for(auto& layer : addLayers)
	{
		Node* lNode = _layer_node(layer);
		fNode->addChild(lNode);
	}
}

void LayerLinkManager::unlinkLayer( const shared_ptr<Layer>& layer )
{
	std::lock_guard<std::mutex> guard(mMutex);

	auto it = mLayerToNodeMapping.find(layer);

	if(it != mLayerToNodeMapping.end())
	{
		Node* lNode = it->second;

		for(auto& p : lNode->participants())
		{
			mParticipantToNodeMapping.erase(p);
		}

		lNode->unlink();
		delete lNode;
	}
}

void LayerLinkManager::unlinkParticipant( const shared_ptr<Participant>& participant )
{
	std::lock_guard<std::mutex> guard(mMutex);

	Node* pNode = _participant_node(participant);
	if(pNode)
		pNode->removeParticipant(participant);
}

std::vector<shared_ptr<Participant>> LayerLinkManager::getLinkedParticipants(const shared_ptr<Layer>& layer) const
{
	std::lock_guard<std::mutex> guard(mMutex);

	std::vector<shared_ptr<Participant>> watchingParticipants;

	auto it = mLayerToNodeMapping.find(layer);
	if(it != mLayerToNodeMapping.end())
	{
		it->second->addWatchingParticipants(watchingParticipants);
	}


	return std::move(watchingParticipants);
}


std::vector<shared_ptr<Layer>> LayerLinkManager::getLinkedLayers( const shared_ptr<Participant>& participant ) const
{
	std::lock_guard<std::mutex> guard(mMutex);

	std::vector<shared_ptr<Layer>> visibleLayers;

	Node* pNode = _participant_node(participant);
	if(pNode)
	{
		pNode->addVisibleLayers(visibleLayers);
	}


	return std::move(visibleLayers);
}



LayerLinkManager& LayerLinkManager::Inst()
{
	static LayerLinkManager inst;
	return inst;
}

LayerLinkManager::Node* LayerLinkManager::_layer_node( const shared_ptr<Layer>& layer )
{
	auto it = mLayerToNodeMapping.find(layer);

	if(it == mLayerToNodeMapping.end())
	{
		std::tie(it, std::ignore) = mLayerToNodeMapping.emplace(layer, new Node(layer));
	}

	return it->second;
}

LayerLinkManager::Node* LayerLinkManager::_participant_node( const shared_ptr<Participant>& participant ) const
{
	auto it = mParticipantToNodeMapping.find(participant);

	return it == mParticipantToNodeMapping.end() ? nullptr : it->second;
}








}