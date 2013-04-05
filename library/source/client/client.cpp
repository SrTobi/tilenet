#include "includes.hpp"

#include "client/client.hpp"


namespace client {



std::unique_ptr<ClientApp> ClientApp::Singleton;



ClientApp::ClientApp()
{
	NOT_IMPLEMENTED();
}

ClientApp::~ClientApp()
{
	NOT_IMPLEMENTED();
}

void ClientApp::start( const shared_ptr<net::ConnectionPort>& port )
{
	NOT_IMPLEMENTED();
}

boost::asio::io_service& ClientApp::service()
{
	return mService;
}

void ClientApp::run()
{
	NOT_IMPLEMENTED();
}


}