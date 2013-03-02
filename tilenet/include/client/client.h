#pragma once
#ifndef _CLIENT_H
#define _CLIENT_H


#include "settings.h"
#include "network/network.h"
#include "network/netReceiver.h"
#include "network/ibuffer.h"


#include "logic/clientLogicAbstraction.h"

namespace client{

class ClientLayer;
class Messenger;

class ClientApp
	:	public net::NetReceiver,
		public utils::enable_shared_for_derived<ClientApp, net::NetReceiver>
{
public:
	enum Status {
		Idle,
		Connecting,
		Connected
	};

public:
	ClientApp(const wstring& host, unsigned short port);
	~ClientApp();


	void lauchInThread();
	
	bool onPackage(net::IBuffer& buffer);
	void onSocketClose();

	void handleUserInput();
	bool connect(const wstring& host, unsigned short port);
	
	void calculateViewport();
	void printMsg(const wstring& text, sf::Color color = sf::Color::White);
	void quit();
	
	inline sf::RenderWindow&	graphics() { return mGraphics; }


	static ClientApp*	Inst() { return Singleton; }

	void run();
private:
	void addLogic(const shared_ptr<ClientLogicAbstraction::Factory>& logic);

private:
	static ClientApp* Singleton;

	// Graphics
	sf::RenderWindow mGraphics;

	bool			mRunning;
	Status			mStatus;

	float			mZoomFactor;
	wstring			mHost;
	unsigned short	mPort;

	// Network
	asio::io_service	mIOService;
	asio::tcp::endpoint	mConnectedEndPoint;

	// Views
	sf::View mStdview;
	sf::View mCalculatedView;

	// Messenger
	boost::scoped_ptr<Messenger> mMessenger;

	// Thread
	boost::thread			mThread;

	shared_ptr<ClientLogicAbstraction>					mCurrentLogic;
	std::map<uint, shared_ptr<ClientLogicAbstraction::Factory> > mLogicCollection;
};


}

#endif
