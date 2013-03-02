#include "includes.h"
#include "server/server.h"
#include "server/display.h"

#include "service/netservice.h"
#include "service/participant.h"

#include "client/client.h"


namespace server {

TilenetServer* TilenetServer::Singleton = nullptr;

/*
TilenetServer::TilenetServer(string serverName, string serverInfo, string tilePackageName, uint   tilePackageVersion,
								string playerPassword, string observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
								bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
								unsigned short portBind, bool setAsActive, bool createStandartLayer)
{
}

TilenetServer::TilenetServer(wstring serverName, wstring serverInfo, wstring tilePackageName, uint tilePackageVersion,
								wstring playerPassword, wstring observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
								bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
								unsigned short portBind, bool setAsActive, bool createStandartLayer)
{
}
*/

#pragma warning(disable: 4355)
TilenetServer::TilenetServer(tn::TilenetConfig& config)
	:	mConfig(config),
		mNextLayerId(0),
		mNextTileId(0)
/*		mPlayer(net::NetManager<User*>::DisconnectHandler(boost::bind(&TilenetServer::handlePlayerDisconnects, this, _1, _2)),
				&TilenetServer::ExtractPackageFromPlayer,
				&mSelector),
		mObserver(net::NetManager<User*>::DisconnectHandler(boost::bind(&TilenetServer::handleObserverDisconnects, this, _1, _2)),
				&TilenetServer::ExtractPackageFromObserver,
				&mSelector),
		mPending(net::NetManager<>::DisconnectHandler(boost::bind(&TilenetServer::handlePendingDisconnects, this, _1, _2)),
				&TilenetServer::ExtractPackageFromPending,
				&mSelector)*/
{
	SLOG(Construct, L"Construct TilenetServer...");
	mTileNames.push_back(L"");
	srand((unsigned int)time(nullptr));
	if(config.setAsActive)
	{
		SLOG(Info, L"Set TilenetServer as active!");
		Singleton = this;
	}


	mService = make_shared<service::NetService>(mConfig, this);
}

TilenetServer::~TilenetServer()
{
	SLOG(Construct, L"Destruct TilenetServer...");
}

uint TilenetServer::tile(const string& name)
{
	return tile(sf::String(name).toWideString());
}

nuint TilenetServer::tile(const wstring& name)
{
	nuint id = 0;
	auto it = mTileIds.find(name);

	if(it == mTileIds.end())
	{
		id = requestTileId();
		SLOG(TileReg, L"Create tile[" << name << "] with id " << id);
		mTileIds.insert(std::make_pair(name, id));
		mTileNames.push_back(name);
	}else{
		id = it->second;
	}

	return id;
}

uint TilenetServer::yield()
{
	mEventService.reset();
	return mEventService.poll();
}

nuint TilenetServer::requestLayerId()
{
	return ++mNextLayerId;
}


nuint TilenetServer::requestTileId()
{
	return ++mNextTileId;
}

tn::Participant* TilenetServer::acquireParticipant(uint p)
{
	shared_ptr<service::Participant> participantPtr = mService->participant(p);

	service::Participant* pp = participantPtr.get();
	
	if(!pp)
		return nullptr;

	pp->holdRef();
	return pp;
}

void TilenetServer::start()
{
	SLOG(Info, L"Start server (Version: " << version::CurrentVersion << ")...");

	mService->start();


	if(!mConfig.standaloneServer)
	{
		SLOG(Info, L"Start singleplayer server...");

		auto app = make_shared<client::ClientApp>(L"", mService->port());
	
		SLOG(Info, L"Start client application...");
		app->lauchInThread();

	}// End: mConfig.maxPlayers == 1 && mConfig.waitForOnePlayerSec
}

unsigned short TilenetServer::port() const
{
	return mService ? mService->port() : 0;
}


shared_ptr<service::NetService> TilenetServer::service() const
{
	return mService;
}

tn::Display* TilenetServer::createDisplay()
{
	shared_ptr<Display> newDisplay = make_shared<Display>(this);
	tn::Display* displayPtr = newDisplay.get();

	mDisplays.insert(std::make_pair(displayPtr, newDisplay));

	return displayPtr;
}

bool TilenetServer::destroyDisplay(tn::Display* display)
{
	auto it = mDisplays.find(display);

	if(it == mDisplays.end())
		return false;

	shared_ptr<Display>& ptr = it->second;

	// Nur loeschen, wenn das wirklich der letzte Zeiger ist!
	if(!ptr.unique())
		return false;

	mDisplays.erase(it);

	return true;
}

bool TilenetServer::registerEventHandler(EventHandler* handler)
{
	if(!handler)
		return false;
	
	auto pair = std::make_pair(handler->eventBitmask(), handler);

	// Wenn der Eventhandler nichts macht, brauchen wir ihn auch nicht registrieren!
	if(pair.first == 0)
		return false;

	mEventHandlers.push_front(pair);

	return true;
}

bool TilenetServer::unregisterEventHandler(EventHandler* handler)
{
	if(!handler)
		return false;

	for(auto it = mEventHandlers.begin();
		it != mEventHandlers.end();
		++it)
	{
		EventHandler* eh = it->second;
		if(eh == handler)
		{
			mEventHandlers.erase(it);
			eh->released();
			return true;
		}
	}

	return false;
}

uint TilenetServer::unregisterAllEventHandlers()
{
	uint releasedHandlers = 0;
	while(mEventHandlers.size())
	{
		EventHandler* eh = mEventHandlers.front().second;
		mEventHandlers.pop_front();
		eh->released();
		++releasedHandlers;
	}

	return releasedHandlers;
}

uint TilenetServer::eventHandlerCount()
{
	return mEventHandlers.size();
}




void TilenetServer::injectEvent_NewParticipant(const shared_ptr<service::Participant>& p)
{
	mEventService.post(boost::bind(&TilenetServer::handleEvent_NewParticipant, this, p));
}

void TilenetServer::injectEvent_ParticipantDisconnected(const shared_ptr<service::Participant>& p)
{
	mEventService.post(boost::bind(&TilenetServer::handleEvent_ParticipantDisconnect, this, p));
}

void TilenetServer::injectEvent_KeyEvent(uint p, const tn::KeyEvent& e)
{
	mEventService.post(boost::bind(&TilenetServer::handleEvent_KeyEvent, this, p, e));
}


#define foreachP(_handler, _bitmask)	\
							EventHandler* _handler = nullptr;											\
							for(auto _it = mEventHandlers.begin(); _it != mEventHandlers.end(); ++_it)	\
								if((_it->first & _bitmask) && (_handler = _it->second))

void TilenetServer::handleEvent_NewParticipant( shared_ptr<service::Participant> p )
{
	foreachP(handler, EventHandler::Bitmask::HandleNewParticipant)
	{
		if(handler->handleNewParticipant(p->id()))
			return;
	}
}

void TilenetServer::handleEvent_ParticipantDisconnect(shared_ptr<service::Participant> p)
{
	foreachP(handler, EventHandler::Bitmask::HandleParticipantDisconnect)
	{
		if(handler->handleParticipantDisconnect(p->id()))
			return;
	}
}

void TilenetServer::handleEvent_KeyEvent(uint p, tn::KeyEvent e )
{
	foreachP(handler, EventHandler::Bitmask::HandleKeyEvent)
	{
		if(handler->handleKeyEvent(p, e))
			return;
	}
}

/*
void TilenetServer::yield(long milliseconds)
{
	sf::Time time = sf::milliseconds(milliseconds);

	if(milliseconds == 0)
		time = sf::microseconds(1);

	if(mSelector.wait(time))
	{
		handleMessageFromPending();
		handleMessageFromPlayer();
		handleMessageFromObserver();
	}
}


void TilenetServer::addPending(SocketPtr& connection)
{
	//SLOG(Info, L"Add a new connection[" << connection->getRemoteAddress() << L"] to pending list");
	mPending.add(connection, nullptr);
}

TilenetServer::ErrorCode TilenetServer::addPlayer(SocketPtr pendingSocket, const string& password)
{
	if(mPlayer.size() >= mConfig.maxPlayers)
	{
		SLOG(Error, L"Can not have more players because server is full!");
		return ServerFull;
	}

	if(password != mConfig.playerPassword)
	{
		SLOG(Error, L"Pending entered wrong password for playing!");
		return WrongPassword;
	}

	//SLOG(Info, L"Moving pending[" << pendingSocket->getRemoteAddress() <<L"] to players...");

	assert(mPending.remove(pendingSocket));

	User* user = new User(pendingSocket, true, this);
	
	mPlayer.add(pendingSocket, user);
	//TODO add palyer
	
	return OK;
}

TilenetServer::ErrorCode TilenetServer::addObserver(SocketPtr pendingSocket, const string& password)
{
	if(mObserver.size() >= mConfig.maxObservers)
	{
		SLOG(Error, L"Can not have more observers because server is full!");
		return ServerFull;
	}

	if(password != mConfig.observerPassword)
	{
		SLOG(Error, L"Pending entered wrong password for observing!");
		return WrongPassword;
	}
	
	mPending.remove(pendingSocket);

	User* user = new User(pendingSocket, false, this);
	
	mObserver.add(pendingSocket, user);
	//TODO add observer

	return OK;
}*/



}
