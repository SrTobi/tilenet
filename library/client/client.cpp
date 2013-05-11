#include "includes.hpp"

#include "client/client.hpp"
#include "client/client_window.hpp"

#include "client/com/com_handler.hpp"
#include "client/com/pv_select.hpp"

namespace client {



std::unique_ptr<ClientApp> ClientApp::Singleton;



ClientApp::ClientApp()
	: log(L"client")
	, mRunning(false)
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
	tnAssert(!mRunning);

	// Setup window and its initialization
	mWindow.reset(new ClientWindow(shared_from_this()));
	mService.post(std::bind(&ClientWindow::init, mWindow));

	mRunning = true;
	std::thread clientThread(&ClientApp::run, shared_from_this());
	clientThread.detach();
}

void ClientApp::stop( bool now /*= false*/ )
{
	mRunning = false;
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
		while(mRunning)
		{
			mService.poll();

			processWindow();
		}
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

const std::shared_ptr<ClientWindow>& ClientApp::window() const
{
	return mWindow;
}

void ClientApp::processWindow()
{
	mWindow->process();
}




}