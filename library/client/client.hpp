#pragma once
#ifndef _CLIENT_HPP
#define _CLIENT_HPP


#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

#include "settings.hpp"
#include "network/connection_port.hpp"

namespace net {
class Message;
}

namespace client {
namespace com {
	class ComHandler;
}

class Messenger;
class ClientWindow;

class ClientApp
	: public std::enable_shared_from_this<ClientApp>
	, boost::noncopyable
{
	typedef boost::asio::basic_waitable_timer<std::chrono::steady_clock> steady_timer;
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
	std::shared_ptr<Messenger> mMessenger;
	std::unique_ptr<boost::asio::io_service::work> mBusyWork;
	steady_timer mWindowProcessTimer;
	float mFrameRate;
	Log	log;


	static std::unique_ptr<ClientApp> Singleton;
};





}










#endif