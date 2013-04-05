#include "includes.hpp"
#include "network/connection_port.hpp"


namespace net {




ConnectionPort::ConnectionPort()
{
}

ConnectionPort::~ConnectionPort()
{
}


void ConnectionPort::setDisconnectHandler( const disconnect_handler_type& func )
{
	mDisconnectHandler = func;
}

void ConnectionPort::setHandler( const handler_type& func )
{
	mHandler = func;
	onHandlerSet();
}

void ConnectionPort::handleReceive( const std::shared_ptr<Message>& msg) const
{
	if(hasHandler())
		mHandler(msg);
}

void ConnectionPort::onHandlerSet()
{
	/* Do nothing */
}

bool ConnectionPort::hasHandler() const
{
	return bool(mHandler);
}

bool ConnectionPort::hasDisconnectHandler() const
{
	return bool(mDisconnectHandler);
}

void ConnectionPort::handleDisconnect() const
{
	if(hasDisconnectHandler())
		handleDisconnect();
}


}
