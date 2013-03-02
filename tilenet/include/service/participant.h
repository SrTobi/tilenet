#pragma once
#ifndef _PARTICIPANT_H
#define _PARTICIPANT_H

#include "settings.h"
#include "network/netReceiver.h"
#include "utils/enable_shared_for_derived.h"
#include "server/display.h"

namespace service {

class NetService;

class Participant
	:	public tn::Participant,
		public net::NetReceiver,
		public utils::enable_shared_for_derived<Participant, net::NetReceiver>
{
public:
	Participant(socket_ptr socket, NetService* service, uint id);
	~Participant();

	//@Override
	bool onPackage(net::IBuffer& buffer);
	
	//@Override
	void onSocketClose();


	//@Override
	bool releaseMe();

	//@Override
	server::Display* attachDisplay(tn::Display* display);

	//@Override
	server::Display* detachDisplay();

	//@Override
	server::Display* getDisplay();
	
	//@Override
	bool kick();

	
	// Halte eine referenz
	void holdRef();

	uint id() const;

private:
	uint			mId;
	NetService*	mService;

	shared_ptr<server::Display>	mDisplay;

	Syncronized<std::stack< shared_ptr<Participant> > >	mSelfAlloc;
};




}





#endif
