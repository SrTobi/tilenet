#pragma once
#ifndef _LISTEN_CONNECTION_PORT_HPP
#define _LISTEN_CONNECTION_PORT_HPP


#include <boost/asio.hpp>

#include "settings.hpp"
#include "connection_port.hpp"


namespace net {

class Message;


class SocketConnectionPort
	: public ConnectionPort
{
public:
	typedef boost::system::error_code error_code;
	typedef shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

	static shared_ptr<SocketConnectionPort> Create(boost::asio::io_service& service, const socket_ptr& socket);
	~SocketConnectionPort();

	virtual OVERRIDE void send(const std::shared_ptr<Message>& msg);

private:
	void startReceive();
	void startBodyReceive();
	void handlePackageHeader(const error_code& err);
	void handleExtendedPackageHeader(const error_code& err);
	void handlePackageBody(const error_code& err);

	virtual OVERRIDE void onMakeDisconnect();
	virtual OVERRIDE bool isConnected() const;


	SocketConnectionPort(boost::asio::io_service& service, const socket_ptr& socket);
private:
	uint8	mDataSizeByte;
	uint32	mDataSize;
	msgid_type mMsgId;
	std::vector<byte> mDataBuffer;
	std::vector<boost::asio::mutable_buffer> mCombinedBuffer;

	boost::asio::io_service& mService;
	socket_ptr mSocket;
};


}



#endif