#include "includes.hpp"

#include <thread>
#include <chrono>

#include "client/client.hpp"
#include "client/client_window.hpp"

#include "client/com/com_handler.hpp"
#include "client/com/pv_select.hpp"

namespace client {



std::unique_ptr<ClientApp> ClientApp::Singleton;



ClientApp::ClientApp()
	: log(L"client")
	, mFrameRate(30.0)
	, mWindowProcessTimer(mService)
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
	tnAssert(!mBusyWork);

	mBusyWork.reset(new boost::asio::io_service::work(mService));
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
	// Setup window and its initialization
	mWindow.reset(new ClientWindow(shared_from_this()));
	mService.post(std::bind(&ClientWindow::init, mWindow));

	// Setup window processing
	mService.post(std::bind(&ClientApp::processWindow, shared_from_this()));

	try {
		tnAssert(mBusyWork);
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

const std::shared_ptr<ClientWindow>& ClientApp::window() const
{
	return mWindow;
}

void ClientApp::processWindow()
{
	using namespace std::chrono;

	milliseconds timePerFrame(int(1.0f / mFrameRate * 1000.0f));

	// process window and measure the time needed
	steady_clock::time_point start = steady_clock::now();
	mWindow->process();
	steady_clock::time_point end = steady_clock::now();

	// call this function again, but wait until one frame time has expired
	auto timeInFrameLeft(timePerFrame - (end - start));

	auto callBinding(std::bind(&ClientApp::processWindow, shared_from_this()));

	if(timeInFrameLeft.count() <= 0)
	{
		mService.post(std::move(callBinding));
	}else{
		mWindowProcessTimer.expires_from_now(timeInFrameLeft);
		mWindowProcessTimer.async_wait(std::move(callBinding));
	}
}




}