#include "includes.hpp"
#include "server/acceptors/listen_acceptor.hpp"


namespace srv {

srv::ListenAcceptor::ListenAcceptor(const shared_ptr<Server>& server, port_type port, uint32 maxConnections )
	: Acceptor(server)
{
	NOT_IMPLEMENTED();
}

srv::ListenAcceptor::~ListenAcceptor()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void srv::ListenAcceptor::start()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void srv::ListenAcceptor::stop()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void srv::ListenAcceptor::destroy()
{
	NOT_IMPLEMENTED();
}


}