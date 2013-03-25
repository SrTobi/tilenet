#pragma once
#ifndef _LISTEN_ACCEPTOR_HPP
#define _LISTEN_ACCEPTOR_HPP

#include "settings.hpp"
#include "server/server.hpp"



struct ListenAcceptor
	: public srv::Acceptor
{
public:
	typedef unsigned short port_type;

	ListenAcceptor(port_type port, uint32 maxConnections);
	~ListenAcceptor();


	virtual override void destroy();
	virtual override shared_ptr<TilenetObject> clone();
private:
	port_type mPort;
	uint32 mMaxConnections;
};




#endif