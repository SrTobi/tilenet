#pragma once
#ifndef _LOCAL_CONNECTION_PORT_HPP
#define _LOCAL_CONNECTION_PORT_HPP

#include <boost/asio/io_service.hpp>

#include "settings.hpp"
#include "connection_port.hpp"


namespace net {

class Message;


class LocalConnectionPort
	: public ConnectionPort
{
public:
	~LocalConnectionPort();

	virtual OVERRIDE void send(const std::shared_ptr<Message>& msg);

	static std::tuple<shared_ptr<LocalConnectionPort>, shared_ptr<LocalConnectionPort>> Create(boost::asio::io_service& s1, boost::asio::io_service& s2);
private:
	LocalConnectionPort(boost::asio::io_service& service);

	virtual OVERRIDE void onMakeDisconnect();
	virtual OVERRIDE bool isConnected() const;


private:
	boost::asio::io_service& mService;
	weak_ptr<LocalConnectionPort> mConnectedPort;
};


}


#endif