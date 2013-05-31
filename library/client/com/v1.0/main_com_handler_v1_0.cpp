#include "includes.hpp"

#include "utils/field.hpp"

#include "main_com_handler_v1_0.hpp"
#include "renderer_v1_0.hpp"

#include "client/client.hpp"
#include "client/client_window.hpp"

#include "network/message.hpp"

namespace client {
namespace com {
namespace v1_0 {




MainComHandler::MainComHandler( ClientApp& app, const shared_ptr<net::ConnectionPort>& port ) : mApp(app)
	, mPort(port)
	, mWindow(app.window())
{
	mDispatcher.add(&MainComHandler::handleLayerControl_attachLayer, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendFullLayer, this);
	mDispatcher.add(&MainComHandler::handleAnswer_StdTileNameRequest, this);
}

void MainComHandler::init()
{
	mTileManager = std::make_shared<TileManager>(std::static_pointer_cast<MainComHandler>(shared_from_this()));
	mRenderer = std::make_shared<Renderer>(mApp.window(), mTileManager);

	mWindow.setLayerRenderer(mRenderer);

	mTileManager->debug_load_test_tileset(L"debug-tileset.txt");
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
	Rect size(msg.width, msg.height);
	Ratio ratio(msg.xratio, msg.yratio);

	mRenderer->defineLayer(msg.layerId, size, ratio);

	// Check content size
	if(msg.layerContent.size() != size.area())
	{
		NOT_IMPLEMENTED();
	}

	for(unsigned int x = 0; x < size.w; ++x)
	{
		for(unsigned int y = 0; y < size.h; ++y)
		{
			Point pos(x,y);
			mRenderer->putTile(msg.layerId, pos, msg.layerContent[pos.fieldIndex(msg.width)]);
		}
	}
}

shared_ptr<MainComHandler> MainComHandler::Create( ClientApp& app, const shared_ptr<net::ConnectionPort>& port )
{
	shared_ptr<MainComHandler> ptr(new MainComHandler(app, port));
	ptr->init();
	return ptr;
}

void MainComHandler::requestStdTileName(TNID tile_id)
{
	if(mRequestedStdTiles.count(tile_id) != 0)
		return;
	mRequestedStdTiles.insert(tile_id);

	proto::v1_0::to_srv::Request_StdTileName req;

	req.tileId = tile_id;

	mPort->send(net::make_message(req));
}

void MainComHandler::handleAnswer_StdTileNameRequest( const proto::v1_0::to_client::Answer_StdTileNameRequest& answ )
{
	mTileManager->identifyStdTile(answ.tileName, answ.tileId);
}








}}}