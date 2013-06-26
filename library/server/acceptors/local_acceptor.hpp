#pragma once
#ifndef _LOCAL_ACCEPTOR_HPP
#define _LOCAL_ACCEPTOR_HPP

#include <chrono>
#include "settings.hpp"
#include "server/server.hpp"
#include "network/connection_port.hpp"

namespace client {
	class ClientApp;
}

namespace srv {

class Service;

struct LocalAcceptor
	: public srv::Acceptor
{
public:

	LocalAcceptor(const shared_ptr<Server>& server);
	~LocalAcceptor();

	void uncouple();

	virtual OVERRIDE void start();
	virtual OVERRIDE void stop();
	virtual OVERRIDE void destroy();

	static void WaitForExit();
	static bool WaitForClientExit(std::chrono::milliseconds& timeout);
private:
	static LocalAcceptor* Singleton;

	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<Server> mServer;
	weak_ptr<client::ClientApp> mClient;
};


}

#endif