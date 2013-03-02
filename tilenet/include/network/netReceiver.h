#pragma once
#ifndef _NETRECEIVER_H
#define _NETRECEIVER_H

#include "settings.h"

#include "ibuffer.h"


namespace net {


// Achtung !!!! muss mit shared_pointer herumgereicht werden
class NetReceiver
	: public boost::enable_shared_from_this<NetReceiver>
{
public:
	NetReceiver();
	NetReceiver(socket_ptr socket);
	virtual ~NetReceiver();
	
	void startReceiving();

	// Bei true werden weitere Packete empfangen
	virtual bool onPackage(net::IBuffer& buffer) = 0;
	virtual void onSocketClose() = 0;
	

	inline void closeSocket() { mSocket->close(); }
	inline const socket_ptr& socket() const { return mSocket; }

protected:
	void setSocket(socket_ptr socket);
private:
	void startReceivingBody();

	void handlePackageHeader(const error_code& err);
	void handleExtendedPackageHeader(const error_code& err);
	void handlePackageBody(const error_code& err); 


private:
	socket_ptr	mSocket;
	IBuffer	mBuffer;
	union {
		sf::Uint32	mDataSize;
		sf::Uint8	mDataSizeByte;
	};
};

}






#endif