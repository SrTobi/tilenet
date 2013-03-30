#include "includes.hpp"
#include "network/connection_port.hpp"


namespace net {




ConnectionPort::ConnectionPort()
{
}

ConnectionPort::~ConnectionPort()
{
}

void ConnectionPort::setHandler( const handler_type& func )
{
	mHandler = func;
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
	return mHandler;
}




}