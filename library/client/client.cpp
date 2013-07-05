#include "includes.hpp"

#include <thread>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "client.hpp"
#include "com_interface.hpp"
#include "messenger.hpp"

#include "client/com/com_handler.hpp"
#include "client/com/pv_select.hpp"

#include "package/package_manager.hpp"

#include "network/socket_connection_port.hpp"

namespace client {


std::future<void> ClientApp::CloseNotifier;
ClientApp* ClientApp::Singleton;



ClientApp::ClientApp(bool asSingleton)
	: log(L"client")
	, mFrameRate(30.0)
	, mWindowProcessTimer(mService)
	, mMessenger(std::make_shared<Messenger>(10, std::chrono::milliseconds(2000), 13))
	, mPackManager(std::make_shared<PackageManager>())
	, mIsSingleton(asSingleton)
{
	if(asSingleton)
	{
		tnAssert(!Singleton);

		// Set future first, because Singleton is checked in WaitForExit first!
		CloseNotifier = mClosePromise.get_future();
		Singleton = this;
	}


	// Add std paths to the package manager (TODO: load it)
	mPackManager->addPackagePath(".");
}

ClientApp::~ClientApp()
{
	if(mIsSingleton)
	{
		Singleton = nullptr;
		mClosePromise.set_value();
	}
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
	disconnect();
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


void ClientApp::connectTo( const string& addr, const string& service )
{
	IMPLEMENTATION_TODO(L"Put this into its own class");
	using namespace boost::asio;
	using namespace boost::asio::ip;
	typedef net::SocketConnectionPort::socket_ptr socket_ptr;

	disconnect();
	
	messenger()->add(L"Try to connect to " + addr);

	// create socket
	socket_ptr socket = std::make_shared<tcp::socket>(mService);

	// try to reach host
	tcp::resolver resolver(mService);
	tcp::resolver::query query(lexical_convert<std::string>(addr), lexical_convert<std::string>(service));
	tcp::resolver::iterator endpoint_iterator;
	boost::system::error_code error;

	endpoint_iterator = resolver.resolve(query, error);

	unsigned short port = 0;
	bool service_found = true;

	if(error)
	{
		log.error() << L"Failed to find a service called '" << service << L"'";

		try {
			port = boost::lexical_cast<unsigned short>(service);

		}catch(std::bad_cast&)
		{
			log.warn() << L"Failed to cast '" << service << "' into a port number!";
			return;
		}

		endpoint_iterator = resolver.resolve(tcp::resolver::query(lexical_convert<std::string>(addr), ""), error);

		service_found = false;

		if(error)
		{
			log.error() << L"Failed to resolve '" << addr << L"'!";
			return;
		}
	}


	tcp::resolver::iterator end;
	error = boost::asio::error::host_not_found;
	while(error && endpoint_iterator != end)
	{
		tcp::endpoint  endpoint(*endpoint_iterator);

		if(!service_found)
			endpoint.port(port);

		socket->close();
		socket->connect(endpoint, error);

		if(error)
		{
			messenger()->add(L"Failed to connect " + lexical_convert<string>(endpoint_iterator->host_name()) + L"!", sf::Color::Yellow);
			++endpoint_iterator;
		}
	}

	if(error || endpoint_iterator == end)
	{
		messenger()->add(L"Failed to connect!!!", sf::Color::Red);
		return;
	}

	//mConnectedEndPoint = *endpoint_iterator;

	messenger()->add(L"Connected to " + lexical_convert<string>(endpoint_iterator->host_name()) + L":" + lexical_convert<string>(endpoint_iterator->service_name()));
	postConnection(net::SocketConnectionPort::Create(mService, socket));
}



void ClientApp::postConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mService.post(std::bind(&ClientApp::handleNewConnection, this, port));
}

void ClientApp::handleNewConnection( const shared_ptr<net::ConnectionPort>& port )
{
	mSecurityViolationSeverity = SecurityViolationKindness;
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
			processEvents(event);
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

void ClientApp::processEvents( const sf::Event& evt )
{
	auto ci = mComInterface.lock();

	switch(evt.type)
	{
	case sf::Event::Closed:
		mWindow.close();
		break;

	case sf::Event::KeyReleased:
	case sf::Event::KeyPressed:
		if(ci && evt.key.code != sf::Keyboard::Unknown)
		{
			ci->notifyKeyevent(evt);
		}
		break;

	case sf::Event::TextEntered:
		if(ci)
		{
			ci->notifyTxtEvent(wchar_t(evt.text.unicode));
		}
		break;

	default:
		break;
	}
}



void ClientApp::onReceive( const shared_ptr<net::Message>& msg )
{
	tnAssert(mComHandler);

	try {
		auto newHandler = mComHandler->handleMessage(msg);
		if(newHandler)
		{
			mComHandler = newHandler;
			mComInterface = mComHandler->getComInterface();
		}

	} catch(const excp::ExceptionBase& e)
	{
		log.error() << lexical_convert<string>(boost::diagnostic_information(e));

		auto* svf = boost::get_error_info<excp::SVFactor>(e);

		if(svf)
			mSecurityViolationSeverity -= *svf;

		if(!svf || mSecurityViolationSeverity <= 0.0f)
		{
			// Ok we had too many errors... disconnect!
			mPort->disconnect();
		}


	} catch(...)
	{
		log.error() << lexical_convert<string>(boost::current_exception_diagnostic_information());
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
