#include "includes.hpp"
#include "network/connection_port.hpp"


namespace net {




ConnectionPort::ConnectionPort()
{
}

ConnectionPort::~ConnectionPort()
{
}

void ConnectionPort::setHandler(const shared_ptr<Handler>& handler)
{
	mHandler = handler;
	onHandlerSet();
}

void ConnectionPort::handleReceive( const std::shared_ptr<Message>& msg) const
{
	auto h = handler();

	if(h)
	{
		h->onReceive(msg);
	}
}

void ConnectionPort::onHandlerSet()
{
	/* Do nothing */
}

shared_ptr<ConnectionPort::Handler> ConnectionPort::handler() const
{
	return mHandler.lock();
}


void ConnectionPort::handleDisconnect()
{
	auto h = handler();

	if(h)
	{
		h->onDisconnect();
	}
}

void ConnectionPort::disconnect()
{
	if(!isConnected())
		return;

	onMakeDisconnect();
	mBinding.reset();
}

void ConnectionPort::bind( const shared_ptr<Handler>& binding )
{
	mBinding = binding;
}




}
