#pragma once
#ifndef _LOCAL_ACCEPTOR_HPP
#define _LOCAL_ACCEPTOR_HPP

#include "settings.hpp"
#include "server/server.hpp"
#include "network/connection_port.hpp"

namespace srv {

class Service;

struct LocalAcceptor
	: public srv::Acceptor
{
public:

	LocalAcceptor(const shared_ptr<Server>& server);
	~LocalAcceptor();

	virtual OVERRIDE void start();
	virtual OVERRIDE void stop();
	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();

private:
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<Server> mServer;
};


}

#endif