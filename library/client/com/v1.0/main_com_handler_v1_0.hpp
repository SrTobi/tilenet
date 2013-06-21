#pragma once
#ifndef _MAIN_COM_HANDLER_V1_0_HPP
#define _MAIN_COM_HANDLER_V1_0_HPP

#include <unordered_set>

#include "settings.hpp"

#include "network/dispatcher.hpp"
#include "client/com/com_handler.hpp"

#include "network/v1.0/protocol_v1_0.hpp"


namespace net {
class ConnectionPort;
}


namespace client {

class ClientApp;
class ClientWindow;


namespace com {
namespace v1_0 {

class ServerInfo;
class TileMapper;
class Renderer;

class MainComHandler
	: public ComHandler
{
	typedef proto::v1_0::to_client::LayerControl_SendLayerUpdate Delta;
	class TileLayerCommitManager;

	MainComHandler(ClientApp& app, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<ServerInfo>& svr_info);
	void init();
public:
	~MainComHandler();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>& msg);

	static shared_ptr<MainComHandler> Create(ClientApp& app, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<ServerInfo>& svr_info);

private:
	virtual OVERRIDE void requestStdTileName(TNID tile_id);


private:
	void handleLayerControl_attachLayer(proto::v1_0::to_client::LayerControl_AttachLayer& msg);
	void handleLayerControl_sendLayerUpdate(proto::v1_0::to_client::LayerControl_SendLayerUpdate& msg);
	void handleLayerControl_sendFullLayer(proto::v1_0::to_client::LayerControl_SendFullLayer& msg);

	void handleAnswer_StdTileNameRequest(proto::v1_0::to_client::Answer_StdTileNameRequest& answ);
private:
	shared_ptr<ComHandler> mNextHandler;
	net::Dispatcher mDispatcher;
	ClientApp& mApp;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<ServerInfo> mServerInfo;
	ClientWindow& mWindow;
	shared_ptr<Renderer> mRenderer;
	shared_ptr<TileMapper> mTileMapper;

	std::unordered_set<TNID> mRequestedStdTiles;
	std::unique_ptr<TileLayerCommitManager> mCommitManager;
};


}}}





#endif