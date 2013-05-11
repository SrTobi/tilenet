#include "includes.hpp"
#include "renderer_v1_0.hpp"


namespace client{
namespace com {
namespace v1_0 {



Renderer::Renderer( const shared_ptr<ClientWindow>& window )
	: mWindow(window)
{
}

Renderer::~Renderer()
{
}

OVERRIDE void Renderer::init( const shared_ptr<sf::RenderTarget>& target )
{
	tnAssert(!mTarget.lock());

	mTarget = target;
}


void Renderer::render()
{
	NOT_IMPLEMENTED();
}


void Renderer::setTopLayer( TNID id )
{
	mTopLayerId = id;

	IMPLEMENTATION_TODO("Clear all layers or something intelligent!");
}

}}}