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




MainComHandler::MainComHandler( const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port )
	: mApp(app)
	, mPort(port)
	, mWindow(app->window())
{
	mDispatcher.add(&MainComHandler::handleLayerControl_attachLayer, this);
	mDispatcher.add(&MainComHandler::handleLayerControl_sendFullLayer, this);
	mDispatcher.add(&MainComHandler::handleAnswer_TileNameRequest, this);
	mDispatcher.add(&MainComHandler::handleAnswer_TilesetNameRequest, this);
}

void MainComHandler::init()
{
	mTileManager = std::make_shared<TileManager>(std::static_pointer_cast<MainComHandler>(shared_from_this()));
	mRenderer = std::make_shared<Renderer>(mApp->window(), mTileManager);

	mWindow->setLayerRenderer(mRenderer);
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

shared_ptr<MainComHandler> MainComHandler::Create( const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port )
{
	shared_ptr<MainComHandler> ptr(new MainComHandler(app, port));
	ptr->init();
	return ptr;
}

void MainComHandler::requestTilesetName( TNID id )
{
	if(mRequestedTilesets.count(id) != 0)
		return;
	mRequestedTilesets.insert(id);

	proto::v1_0::to_srv::Request_TilesetName req;

	req.tilesetId = id;

	mPort->send(net::make_message(req));
}

void MainComHandler::requestStdIdTileName(TNID tile_id, TNID tileset_id)
{
	auto id_pair = std::make_pair(tile_id, tileset_id);
	if(mRequestedTiles.count(id_pair) != 0)
		return;
	mRequestedTiles.insert(id_pair);

	proto::v1_0::to_srv::Request_TileName req;

	req.tilesetId = tileset_id;
	req.tileId = tile_id;

	mPort->send(net::make_message(req));
}

void MainComHandler::handleAnswer_TileNameRequest( const proto::v1_0::to_client::Answer_TileNameRequest& answ )
{
	mTileManager->identifyStdIdTile(answ.tilesetId, answ.tileName, answ.tileId);
}

void MainComHandler::handleAnswer_TilesetNameRequest( const proto::v1_0::to_client::Answer_TilesetNameRequest& answ )
{
	mTileManager->identifyTileset(answ.tilesetName, answ.tilesetId);
}








}}}