#pragma once
#ifndef _SERVER_H
#define _SERVER_H


#include "settings.h"
#include "network/network.h"

namespace service {
	class Participant;
	class NetService;
}

namespace server {
class Display;

class TilenetServer
	: public tn::Tilenet
{
	enum ErrorCode {
		Nothing,
		OK,
		ServerFull,
		WrongPassword
	};
public:
	TilenetServer(tn::TilenetConfig& config);
	~TilenetServer();

	uint tile(const string& name);
	uint tile(const wstring& name);


	//@Override
	tn::Participant* acquireParticipant(uint p);

	//@Override
	void start();
	
	//@Override
	unsigned short port() const;
	
	//@Override
	tn::Display* createDisplay();

	//@Override
	bool destroyDisplay(tn::Display* display);
	
	//@Override
	uint yield();

	//@Override
	bool registerEventHandler(EventHandler* handler);

	//@Override
	bool unregisterEventHandler(EventHandler* handler);

	//@Override
	uint unregisterAllEventHandlers();

	//@Override
	uint eventHandlerCount();


	nuint requestLayerId();
	nuint requestTileId();
	
	shared_ptr<service::NetService> service() const;
	

	void injectEvent_NewParticipant(const shared_ptr<service::Participant>& p);
	void injectEvent_ParticipantDisconnected(const shared_ptr<service::Participant>& participant);
	void injectEvent_KeyEvent(uint p, const tn::KeyEvent& e);

	const wstring& resolveTileId(uint id) const { return mTileNames.at(id); }

	static TilenetServer* Inst() { return Singleton; }

private:

	void handleEvent_NewParticipant(shared_ptr<service::Participant> p);
	void handleEvent_ParticipantDisconnect(shared_ptr<service::Participant> participant);
	void handleEvent_KeyEvent(uint p, tn::KeyEvent e);


private:
	static TilenetServer* Singleton;

	nuint					mNextLayerId;
	nuint					mNextTileId;

	std::unordered_map<nstring, nuint>	mTileIds;
	std::vector<nstring>				mTileNames;

	shared_ptr<service::NetService>	mService;

	const TilenetConfig		mConfig;

	std::map<tn::Display*, shared_ptr<Display> > mDisplays;

	// Events
	asio::io_service										mEventService;
	std::list<std::pair<uint, tn::Tilenet::EventHandler*> >	mEventHandlers;
	//SocketPtr				mMainServer;
};

}

#endif
