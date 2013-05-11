#include "includes.hpp"

#include "main_com_handler_v1_0.hpp"
#include "renderer_v1_0.hpp"

#include "client/client.hpp"
#include "client/client_window.hpp"

#include "renderer_v1_0.hpp"


namespace client {
namespace com {
namespace v1_0 {




MainComHandler::MainComHandler( const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port )
	: mApp(app)
	, mPort(port)
	, mWindow(app->window())
	, mRenderer(new Renderer(app->window()))
{
	mWindow->setLayerRenderer(mRenderer);

	mDispatcher.add(&MainComHandler::handleLayerControl_attachLayer, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendFullLayer, this);
}

MainComHandler::~MainComHandler()
{
}

OVERRIDE shared_ptr<ComHandler> MainComHandler::handleMessage(const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return shared_ptr<ComHandler>();
}

void MainComHandler::handleLayerControl_attachLayer( const proto::v1_0::to_client::LayerControl_AttachLayer& msg )
{
	mRenderer->setTopLayer(msg.layerId);
}

void MainComHandler::handleLayerControl_sendFullLayer( const proto::v1_0::to_client::LayerControl_SendFullLayer& msg )
{
	IMPLEMENTATION_TODO("SendFullLayer is not implemented!");
}



}}}