#include "includes.hpp"
#include "server/acceptors/local_acceptor.hpp"
#include "server/service.hpp"

#include "network/local_connection_port.hpp"

#include "client/client.hpp"

namespace srv {

LocalAcceptor::LocalAcceptor()
{
}

LocalAcceptor::~LocalAcceptor()
{
}

OVERRIDE void LocalAcceptor::start()
{
	shared_ptr<client::ClientApp> cl(new client::ClientApp());
	shared_ptr<net::ConnectionPort> theirPort;

	std::tie(mPort, theirPort) = net::LocalConnectionPort::Create(Service::Inst(), cl->service());

	cl->start(mPort);
}

OVERRIDE void LocalAcceptor::stop()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void LocalAcceptor::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> LocalAcceptor::clone()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Local acceptor can not be cloned!"));
}


}