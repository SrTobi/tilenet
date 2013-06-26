#include "includes.hpp"
#include "server/acceptors/local_acceptor.hpp"
#include "server/service.hpp"

#include "network/local_connection_port.hpp"

#include "client/client.hpp"

namespace srv {

LocalAcceptor* LocalAcceptor::Singleton = 0;

LocalAcceptor::LocalAcceptor(const shared_ptr<Server>& server)
	: Acceptor(server)
{
	if(Singleton)
	{
		BOOST_THROW_EXCEPTION(excp::AlreadyExistsException() << excp::InfoWhat(L"A local acceptor was already created!"));
	}

	Singleton = this;
}

LocalAcceptor::~LocalAcceptor()
{
	Singleton = nullptr;
}

OVERRIDE void LocalAcceptor::start()
{
	shared_ptr<client::ClientApp> cl = std::make_shared<client::ClientApp>(true);
	shared_ptr<net::ConnectionPort> theirPort;

	mClient = cl;

	std::tie(mPort, theirPort) = net::LocalConnectionPort::Create(Service::Inst(), cl->service());

	cl->postConnection(theirPort);
	cl->start();
	server()->addParticipant(mPort);
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


void LocalAcceptor::uncouple()
{
	mClient.reset();
}


void LocalAcceptor::WaitForExit()
{
	client::ClientApp::WaitForExit();
}


bool LocalAcceptor::WaitForClientExit(std::chrono::milliseconds& timeout)
{
	return client::ClientApp::WaitForExit(timeout);
}




}