#pragma once
#ifndef _PENDING_H
#define _PENDING_H

#include "settings.h"

#include "utils/enable_shared_for_derived.h"

#include "network/netReceiver.h"
#include "network/network.h"


namespace service {


class NetService;

class PendingConnection
	:	public net::NetReceiver,
		public utils::enable_shared_for_derived<PendingConnection, net::NetReceiver>
{
public:
	PendingConnection(socket_ptr socket, NetService* service);
	~PendingConnection();

	//@Oberride
	bool onPackage(net::IBuffer& buffer);
	
	//@Oberride
	void onSocketClose();

	void sendWelcomeHandshake();

private:
	bool handleCollection(net::IBuffer::Map& request);


	
	NetService*	mService;
};

}

#endif
