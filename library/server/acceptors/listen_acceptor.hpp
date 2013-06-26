#pragma once
#ifndef _LISTEN_ACCEPTOR_HPP
#define _LISTEN_ACCEPTOR_HPP

#include "settings.hpp"
#include "server/server.hpp"

namespace srv {

class Service;

struct ListenAcceptor
	: public srv::Acceptor
{
public:
	typedef unsigned short port_type;

	ListenAcceptor(const shared_ptr<Server>& server, port_type port, uint32 maxConnections);
	~ListenAcceptor();

	virtual OVERRIDE void start();
	virtual OVERRIDE void stop();
	virtual OVERRIDE void destroy();
private:
	port_type mPort;
	uint32 mMaxConnections;
	shared_ptr<Service> mService;
};


}

#endif