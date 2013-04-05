#pragma once
#ifndef _CLIENT_HPP
#define _CLIENT_HPP


#include <boost/asio/io_service.hpp>

#include "settings.hpp"
#include "network/connection_port.hpp"

namespace client {


class ClientApp
	: public std::enable_shared_from_this<ClientApp>
{
public:
	ClientApp();
	~ClientApp();

	void start();
	void start(const shared_ptr<net::ConnectionPort>& port);

private:
	void run();

private:
	boost::asio::io_service mService;
	shared_ptr<net::ConnectionPort> mPort;
};





}










#endif