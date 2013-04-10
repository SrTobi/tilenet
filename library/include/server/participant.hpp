#pragma once
#ifndef _PARTICIPANT_HPP
#define _PARTICIPANT_HPP


#include "settings.hpp"

namespace net {
	class ConnectionPort;
	class Message;
}

namespace srv {

class EventQueue;

class Participant
	: public std::enable_shared_from_this<Participant>
	, public IdObject<Participant>
{
private:
	class StatusHandler;
	class HandshakeStatusHandler;
public:
	Participant(const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port);
	~Participant();

	void kick(const string& reason);
private:
	void handleMessage(const shared_ptr<net::Message>& msg);
	void handleDisconnect();

private:
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<EventQueue> mEventQueue;
	std::unique_ptr<StatusHandler> mHandler;
};


}



#endif