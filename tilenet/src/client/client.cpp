#include "includes.h"
#include "client/client.h"
#include "client/messenger.h"
#include "client/stdTilePool.h"

#include "client/logic/v1/clientLogic.h"

namespace client {

ClientApp* ClientApp::Singleton = nullptr;




ClientApp::ClientApp(const wstring& host, unsigned short port)
	:	net::NetReceiver(),
		mRunning(false),
		mStatus(Idle),
		mZoomFactor(1.0f),
		mHost(host),
		mPort(port)
{
	setSocket(make_shared<asio::tcp::socket>(mIOService));
	CLOG(Construct, L"Construct Client application");
	if(Singleton)
	{
		CLOG(Error, L"A Client has already been created. You can not have two!");
		throw tn::TnException(L"Can not create two Clients!");
	}
	Singleton = this;

	mMessenger.reset(new Messenger(mGraphics, logg::_tnClientLogOutput, 10, 5000, 12));

	// Fuege Logiken hinzu:
	addLogic(boost::static_pointer_cast<ClientLogicAbstraction::Factory>(
					make_shared<ClientLogicAbstraction::TemplateFactory<v1::ClientLogic> >(1)
					));

}

ClientApp::~ClientApp()
{
	setSocket(socket_ptr());
	Singleton = nullptr;
}


bool ClientApp::connect(const wstring& host, unsigned short port)
{
	
	CLOG(Info, L"Resolve " << host << L":" << port);

	// Versuche den Host zu erreichen
	asio::tcp::resolver resolver(mIOService);
	asio::tcp::resolver::query query(sf::String(host).toAnsiString(), boost::lexical_cast<string>(port));
	asio::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	asio::tcp::resolver::iterator end;

	boost::system::error_code error = boost::asio::error::host_not_found;
	while(error && endpoint_iterator != end)
	{
		socket()->close();
		socket()->connect(*endpoint_iterator++, error);

		if(error)
		{
			CLOG(Warning, L"Failed to connect " << endpoint_iterator->host_name().c_str() << L"!");
		}
	}
	if(error || endpoint_iterator == end)
	{
		CLOG(Error, L"Failed to connect to any endpoint!");
		return false;
	}

	//mConnectedEndPoint = *endpoint_iterator;

	CLOG(Info, L"Connected to " << endpoint_iterator->host_name().c_str()
				<< L":" << endpoint_iterator->service_name().c_str());

	mStatus = Connecting;
	startReceiving();

	return true;
}


void ClientApp::handleUserInput()
{
	sf::Event event;
	while (mGraphics.pollEvent(event))
	{
		// Close window : exit
		switch(event.type)
		{
		case sf::Event::Closed:
			{
				quit();
			}break;
		case sf::Event::Resized:
			{
				calculateViewport();
			}break;
		case sf::Event::MouseWheelMoved:
			{
				mZoomFactor *= event.mouseWheel.delta > 0 ? 2.0f : 0.5f;
				calculateViewport();
			}break;
		case sf::Event::KeyPressed:
			{
				if(mCurrentLogic)
					mCurrentLogic->onKeyEvent(event.key, true);
			}break;
		case sf::Event::KeyReleased:
			{
				if(mCurrentLogic)
					mCurrentLogic->onKeyEvent(event.key, false);
			}break;
		default:
			break;
		}
	}
}



void ClientApp::lauchInThread()
{
	boost::thread(&ClientApp::run, shared_from_derived_this());
}


void ClientApp::run()
{
	mRunning = true;

	if(!connect(mHost, mPort))
	{
		CLOG(Error, L"Was not able to connect to any server!");
		throw tn::TnException(L"Can not connect!");
	}

	mGraphics.create(sf::VideoMode(800, 600, 32), "Tilenet");
	mStdview = mGraphics.getDefaultView();


	mGraphics.setFramerateLimit(30);

	// init standard tileset
	StdTilePool::Inst();


	CLOG(Info, L"Start running the client...");
	printMsg(L"Here we go...");
	while(mRunning)
	{
		// Events abfangen
		mIOService.poll();
		handleUserInput();

		mGraphics.clear();
		mGraphics.setView(mCalculatedView);
		if(mCurrentLogic)
			mCurrentLogic->onRender();

		mGraphics.setView(mStdview);
		mMessenger->render();
		mGraphics.display();
	}

	CLOG(Info, "Client stops.");
	mGraphics.close();
	

	closeSocket();
	mIOService.run();
	
	mCurrentLogic.reset();
	// Peace :)
	//delete this;
}



bool ClientApp::onPackage(net::IBuffer& buffer)
{
	if(mCurrentLogic)
		mCurrentLogic->onNetworkPackage(buffer);
	else{

		if(buffer.id() != net::msg::Collection)
		{
			CLOG(Error, L"Unknown message[" << buffer.id() << L"] context, because no handshake was done!");
			return false;
		}

		net::Protocol<net::msg::Collection> protocol(buffer);
		net::IBuffer::Map& content = protocol.collection;

		if(content["subject"] != L"handshake")
		{
			CLOG(Error, L"Unknown message before handshake!");
			return false;
		}

		uint version = 0;

		try {
			version = boost::lexical_cast<uint>(content["version"]);
		}catch(boost::bad_lexical_cast&)
		{
			CLOG(Error, L"Unknown version-format in handshake!");
			return false;
		}

		auto it = mLogicCollection.find(version);
		
		if(it == mLogicCollection.end())
		{
			CLOG(Error, L"This client is not able to handle the protocol version[" << version << "]");
			return false;
		}

		printMsg(L"Connected!");

		CLOG(Info, L"Create communication-layer with version " << version);
		shared_ptr<ClientLogicAbstraction::Factory> factory = it->second;

		mCurrentLogic = factory->build(content, shared_from_derived_this());

		if(mCurrentLogic)
		{
			CLOG(Info, L"Successfully connected!");
			mStatus = Connected;
		}else{
			CLOG(Error, L"Failed to create communication-layer!");
			return false;
		}
	}

	return true;
}


void ClientApp::onSocketClose()
{
	// Verbinfung verloren
	CLOG(Info, L"Client is idle!");
	mStatus = Idle;
}



void ClientApp::quit()
{
	mRunning = false;
}

void ClientApp::addLogic(const shared_ptr<ClientLogicAbstraction::Factory>& logic)
{
	mLogicCollection.insert(std::make_pair(logic->version(), logic));
}


void ClientApp::calculateViewport()
{
	if(!mCurrentLogic)
		return;

	//TODO Besser machen
	std::pair<Point, Point> bounds = mCurrentLogic->getBounds();

	Point& topLeft		= bounds.first;
	Point& bottomRight	= bounds.second;

	Rect size(bottomRight - topLeft);

	mCalculatedView.reset(sf::FloatRect(float(topLeft.x),
										float(topLeft.y),
										float(size.x),
										float(size.y)));

	sf::Vector2u winSize = mGraphics.getSize();

	tn::Vector rsize(float(size.w) / float(winSize.x),
					 float(size.h) / float(winSize.y));

	// middle - size/2
	mCalculatedView.setViewport(sf::FloatRect(	0.5f - mZoomFactor * rsize.x / (2.0f),
												0.5f - mZoomFactor * rsize.y / (2.0f),
																//0.5f + rsize.x / 2.0f,
																//0.5f + rsize.y / 2.0f));
												mZoomFactor * rsize.x, mZoomFactor * rsize.y));
}

void ClientApp::printMsg(const wstring& text, sf::Color color /*= sf::Color::White*/ )
{
	mMessenger->add(text, color);
}

}
