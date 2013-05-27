#pragma once
#ifndef _MAIN_COM_HANDLER_V1_0_HPP
#define _MAIN_COM_HANDLER_V1_0_HPP

#include <unordered_set>

#include "settings.hpp"

#include "network/dispatcher.hpp"
#include "client/com/com_handler.hpp"

#include "client/tile_manager.hpp"
#include "network/v1.0/protocol_v1_0.hpp"


namespace net {
class ConnectionPort;
}


namespace client {

class ClientApp;
class ClientWindow;


namespace com {
namespace v1_0 {

class Renderer;

class MainComHandler
	: public ComHandler
	, public TileManager::RequestService
{
	MainComHandler(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port);
	void init();
public:
	~MainComHandler();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>& msg);

	static shared_ptr<MainComHandler> Create(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port);

private:
	virtual OVERRIDE void requestTilesetName( TNID id );
	virtual OVERRIDE void requestStdIdTileName(TNID tile_id, TNID tileset_id );


private:
	void handleLayerControl_attachLayer(const proto::v1_0::to_client::LayerControl_AttachLayer& msg);
	void handleLayerControl_sendFullLayer(const proto::v1_0::to_client::LayerControl_SendFullLayer& msg);

	void handleAnswer_TileNameRequest(const proto::v1_0::to_client::Answer_TileNameRequest& answ);
	void handleAnswer_TilesetNameRequest(const proto::v1_0::to_client::Answer_TilesetNameRequest& answ);
private:
	shared_ptr<ComHandler> mNextHandler;
	net::Dispatcher mDispatcher;
	shared_ptr<ClientApp> mApp;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<ClientWindow> mWindow;
	shared_ptr<Renderer> mRenderer;
	shared_ptr<TileManager> mTileManager;

	std::unordered_set<TNID> mRequestedTilesets;
	std::unordered_set<std::pair<TNID, TNID>> mRequestedTiles;
};


}}}





#endif