#include "includes.hpp"
#include "server/acceptors/local_acceptor.hpp"
#include "server/service.hpp"

#include "network/local_connection_port.hpp"

#include "client/client.hpp"

namespace srv {

LocalAcceptor* LocalAcceptor::Singleton = 0;

LocalAcceptor::LocalAcceptor(const shared_ptr<Server>& server)
	: mServer(server)
{
	if(Singleton)
	{
		NOT_IMPLEMENTED();
	}

	Singleton = this;
}

LocalAcceptor::~LocalAcceptor()
{
	Singleton = nullptr;
}

OVERRIDE void LocalAcceptor::start()
{
	shared_ptr<client::ClientApp> cl = std::make_shared<client::ClientApp>();
	shared_ptr<net::ConnectionPort> theirPort;

	mClient = cl;

	std::tie(mPort, theirPort) = net::LocalConnectionPort::Create(Service::Inst(), cl->service());

	cl->postConnection(theirPort);
	cl->start();
	mServer->addParticipant(mPort);
}

OVERRIDE void LocalAcceptor::stop()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void LocalAcceptor::destroy()
{
	auto cl = mClient.lock();

	if(cl)
		cl->stop(true);
}

OVERRIDE shared_ptr<TilenetObject> LocalAcceptor::clone()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Local acceptor can not be cloned!"));
}

void LocalAcceptor::uncouple()
{
	mClient.reset();
}

void LocalAcceptor::WaitForClientExit()
{
	client::ClientApp::WaitForExit();
}




}