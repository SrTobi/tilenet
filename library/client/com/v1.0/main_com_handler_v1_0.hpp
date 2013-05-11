#pragma once
#ifndef _MAIN_COM_HANDLER_V1_0_HPP
#define _MAIN_COM_HANDLER_V1_0_HPP

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

class Renderer;

class MainComHandler
	: public ComHandler
{
public:
	MainComHandler(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port);
	~MainComHandler();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>& msg);
private:
	void handleLayerControl_attachLayer(const proto::v1_0::to_client::LayerControl_AttachLayer& msg);
	void handleLayerControl_sendFullLayer(const proto::v1_0::to_client::LayerControl_SendFullLayer& msg);


private:
	shared_ptr<ComHandler> mNextHandler;
	net::Dispatcher mDispatcher;
	const shared_ptr<ClientApp> mApp;
	const shared_ptr<net::ConnectionPort> mPort;
	const shared_ptr<ClientWindow> mWindow;
	const shared_ptr<Renderer> mRenderer;
};


}}}





#endif