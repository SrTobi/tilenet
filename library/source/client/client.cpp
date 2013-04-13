#include "includes.hpp"

#include "client/client.hpp"

#include "client/com/com_handler.hpp"
#include "client/com/pv_select.hpp"

namespace client {



std::unique_ptr<ClientApp> ClientApp::Singleton;



ClientApp::ClientApp()
	: log(L"client")
{
	if(Singleton)
	{
		// Client already exists
		NOT_IMPLEMENTED();
	}
	Singleton.reset(this);
}

ClientApp::~ClientApp()
{
	Singleton.reset();
}

void ClientApp::start()
{
	std::thread clientThread(&ClientApp::run, shared_from_this());
	clientThread.detach();
}

void ClientApp::stop( bool now /*= false*/ )
{
	mBusyWork.reset();
	if(now)
		mService.stop();
}

boost::asio::io_service& ClientApp::service()
{
	return mService;
}

void ClientApp::run()
{
	try {
		mBusyWork.reset(new boost::asio::io_service::work(mService));

		mService.run();
	}catch(...)
	{
		log.error()
			<< L"Exception in worker thread!"
			<< L"\n------------------------\n"
			<< lexical_convert<string>(boost::current_exception_diagnostic_information())
			<< L"\n------------------------";
	}
}

void ClientApp::postConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mService.post(std::bind(&ClientApp::handleNewConnection, shared_from_this(), port));
}

void ClientApp::handleNewConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mPort = port;
	mComHandler.reset(new com::ProtocolVersionSelect(shared_from_this(), port));
	mPort->setHandler(std::bind(&ClientApp::handleMessage, shared_from_this(), std::placeholders::_1));
}

void ClientApp::handleMessage( const shared_ptr<net::Message>& msg )
{
	tnAssert(mComHandler);
	auto newHandler = mComHandler->handleMessage(msg);
	if(newHandler)
		mComHandler = newHandler;
}


}