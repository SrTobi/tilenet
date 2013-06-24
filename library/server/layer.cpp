#include "includes.hpp"
#include "server/layer.hpp"

#include "layer_link_manager.hpp"

namespace srv {


Layer::Layer()
	: mCommit(0)
{

}

Layer::~Layer()
{

}

TNID Layer::currentCommitNr() const
{
	return mCommit;
}

TNID Layer::newCommit()
{
	return ++mCommit;
}

void Layer::init()
{
	LayerLinkManager::Inst().registerLayer(self<Layer>());
}

OVERRIDE void Layer::destroy()
{
	LayerLinkManager::Inst().unregisterLayer(self<Layer>());
}





}