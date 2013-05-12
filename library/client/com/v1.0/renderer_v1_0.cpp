#include "includes.hpp"
#include "renderer_v1_0.hpp"


namespace client{
namespace com {
namespace v1_0 {

class Renderer::Layer
{
public:

	virtual void render(sf::RenderTarget& target) = 0;

private:
};











Renderer::Renderer( const shared_ptr<ClientWindow>& window )
	: mWindow(window)
{
}

Renderer::~Renderer()
{
}


void Renderer::render(sf::RenderTarget& target)
{
	// Set background
	target.clear(mBGCOlor);

	// Render layers
	auto topLayer = layer(mTopLayerId);

	if(topLayer)
		topLayer->render(target);
}


void Renderer::setTopLayer( TNID id )
{
	mTopLayerId = id;

	IMPLEMENTATION_TODO("Clear all layers or something intelligent!");
}

shared_ptr<Renderer::Layer> Renderer::layer( TNID id ) const
{
	auto it = mIdToLayerMapping.find(id);

	if(it == mIdToLayerMapping.end())
		return shared_ptr<Layer>();

	return it->second;
}

}}}