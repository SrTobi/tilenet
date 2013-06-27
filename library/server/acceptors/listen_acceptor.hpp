#pragma once
#ifndef _LISTEN_ACCEPTOR_HPP
#define _LISTEN_ACCEPTOR_HPP


#include <boost/asio.hpp>
#include "settings.hpp"
#include "server/server.hpp"

namespace srv {

class Service;

struct ListenAcceptor
	: public srv::Acceptor
{
public:
	typedef shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
	typedef unsigned short port_type;

	ListenAcceptor(const shared_ptr<Server>& server, port_type port, uint32 maxConnections);
	~ListenAcceptor();

	virtual OVERRIDE void start();
	virtual OVERRIDE void stop();
	virtual OVERRIDE void destroy();

private:
	void startAsyncAccept();
	void handleAsyncAccept(const socket_ptr& socket, const boost::system::error_code& err);

private:
	port_type mPort;
	uint32 mMaxConnections;
	shared_ptr<Service> mService;
	boost::asio::ip::tcp::acceptor mAcceptor;
};


}

#endif