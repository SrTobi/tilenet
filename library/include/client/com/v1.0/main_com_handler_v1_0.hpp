#pragma once
#ifndef _MAIN_COM_HANDLER_V1_0_HPP
#define _MAIN_COM_HANDLER_V1_0_HPP

#include "settings.hpp"

#include "network/dispatcher.hpp"
#include "client/com/com_handler.hpp"


namespace net {
class ConnectionPort;
}


namespace client {
class ClientApp;

namespace com {
namespace v1_0 {

	
class MainComHandler
	: public ComHandler
{
public:
	MainComHandler(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port);
	~MainComHandler();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>& msg);
private:

private:
	shared_ptr<ComHandler> mNextHandler;
	net::Dispatcher mDispatcher;
	const shared_ptr<ClientApp> mApp;
	const shared_ptr<net::ConnectionPort> mPort;
};


}}}





#endif