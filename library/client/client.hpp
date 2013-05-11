#pragma once
#ifndef _CLIENT_HPP
#define _CLIENT_HPP


#include <boost/asio/io_service.hpp>

#include "settings.hpp"
#include "network/connection_port.hpp"

namespace net {
class Message;
}

namespace client {
namespace com {
	class ComHandler;
}


class ClientWindow;

class ClientApp
	: public std::enable_shared_from_this<ClientApp>
{
public:
	ClientApp();
	~ClientApp();

	void start();
	void postConnection(const shared_ptr<net::ConnectionPort>& port);
	void stop(bool now = false);

	const std::shared_ptr<ClientWindow>& window() const;

	boost::asio::io_service& service();
private:
	void run();
	void handleNewConnection(const shared_ptr<net::ConnectionPort>& port);
	void handleMessage(const shared_ptr<net::Message>& msg);
	void processWindow();
private:
	boost::asio::io_service mService;
	std::shared_ptr<com::ComHandler> mComHandler;
	std::shared_ptr<net::ConnectionPort> mPort;
	std::shared_ptr<ClientWindow> mWindow;
	Log	log;
	bool mRunning;

	static std::unique_ptr<ClientApp> Singleton;
};





}










#endif