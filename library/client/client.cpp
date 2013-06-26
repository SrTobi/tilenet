#include "includes.hpp"

#include <thread>
#include <chrono>

#include "client.hpp"
#include "com_interface.hpp"
#include "messenger.hpp"

#include "client/com/com_handler.hpp"
#include "client/com/pv_select.hpp"

#include "package/package_manager.hpp"

namespace client {


std::future<void> ClientApp::CloseNotifier;
ClientApp* ClientApp::Singleton;



ClientApp::ClientApp()
	: log(L"client")
	, mFrameRate(30.0)
	, mWindowProcessTimer(mService)
	, mMessenger(std::make_shared<Messenger>(10, std::chrono::milliseconds(2000), 13))
	, mPackManager(std::make_shared<PackageManager>())
{
	if(Singleton)
	{
		// Client already exists
		NOT_IMPLEMENTED();
	}

	// Set future first, because Singleton is checked in WaitForExit first!
	CloseNotifier = mClosePromise.get_future();
	Singleton = this;

	// Add std paths to the package manager (TODO: load it)
	mPackManager->addPackagePath(".");
}

ClientApp::~ClientApp()
{
	Singleton = nullptr;
	mClosePromise.set_value();
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

void ClientApp::init()
{
	mWindow.create(sf::VideoMode(800, 600), "tilenet");

	if(!mWindow.isOpen())
	{
		stop();
	}
}


void ClientApp::run()
{
	// Setup window and its initialization
	mService.post(std::bind(&ClientApp::init, shared_from_this()));

	// Search for packages
	mPackManager->serachPackages();

	// Setup window processing
	mService.post(std::bind(&ClientApp::processWindow, shared_from_this()));

	try {
		mService.run();
		disconnect();
	} catch(...) {
		log.error()
			<< L"Exception in client thread!"
			<< L"\n------------------------\n"
			<< lexical_convert<string>(boost::current_exception_diagnostic_information())
			<< L"\n------------------------";

#ifdef TILENET_RETHROW_THREAD_EXCEPTIONS
		throw;
#endif
	}
}


void ClientApp::disconnect()
{
	if(mPort)
		mPort->disconnect();
	mPort.reset();
}



void ClientApp::postConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mService.post(std::bind(&ClientApp::handleNewConnection, this, port));
}

void ClientApp::handleNewConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mPort = port;
	mComHandler = std::make_shared<com::ProtocolVersionSelect>(*this, port);
	mPort->setHandler(shared_from_this());
}

const shared_ptr<net::ConnectionPort>& ClientApp::port() const
{
	return mPort;
}

const shared_ptr<PackageManager>& ClientApp::pmanager() const
{
	return mPackManager;
}

sf::RenderWindow& ClientApp::window()
{
	return mWindow;
}


const shared_ptr<Messenger>& ClientApp::messenger() const
{
	return mMessenger;
}

void ClientApp::processWindow()
{
	using namespace std::chrono;

	milliseconds timePerFrame(int(1.0f / mFrameRate * 1000.0f));

	// process window and measure the time needed
	steady_clock::time_point start = steady_clock::now();

	{
		auto ci = mComInterface.lock();
		if(!mWindow.isOpen())
		{
			stop();
		}

		// Process event loop
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mWindow.close();
			if(ci && (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) && event.key.code != sf::Keyboard::Unknown)
			{
				ci->notifyKeyevent(event);
			}
		}

		mWindow.clear();

		// If a renderer is set, then render 
		if(ci)
		{
			ci->notifyRender(mWindow);
		}



		mMessenger->render(mWindow);
		mWindow.display();
	}

	steady_clock::time_point end = steady_clock::now();

	// call this function again, but wait until one frame time has expired
	auto timeInFrameLeft(timePerFrame - (end - start));

	if(!mBusyWork)
		return;

	auto callBinding(std::bind(&ClientApp::processWindow, shared_from_this()));

	if(timeInFrameLeft.count() <= 0)
	{
		mService.post(std::move(callBinding));
	}else{
		mWindowProcessTimer.expires_from_now(timeInFrameLeft);
		mWindowProcessTimer.async_wait(std::move(callBinding));
	}
}


void ClientApp::onReceive( const shared_ptr<net::Message>& msg )
{
	tnAssert(mComHandler);
	auto newHandler = mComHandler->handleMessage(msg);
	if(newHandler)
	{
		mComHandler = newHandler;
		mComInterface = mComHandler->getComInterface();
	}
}

OVERRIDE void ClientApp::onDisconnect()
{
	mMessenger->add(L"Disconnected!!!", sf::Color::Red);
	mComInterface.reset();
	mComHandler.reset();
}

void ClientApp::WaitForExit()
{
	if(Singleton)
		CloseNotifier.wait();
}


bool ClientApp::WaitForExit(std::chrono::milliseconds& timeout)
{
	if(Singleton)
	{		
		auto start_time = std::chrono::system_clock::now();
		bool exited = (CloseNotifier.wait_for(timeout) != std::future_status::timeout) && !Singleton;

		if(exited)
		{
			timeout -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time); 
		}else{
			timeout = std::chrono::milliseconds(0);
		}


		return exited;
	}
	return true;
}










}
