#pragma once
#ifndef _PARTICIPANT_HPP
#define _PARTICIPANT_HPP


#include "settings.hpp"

namespace net {
	class ConnectionPort;
}

namespace srv {

class EventQueue;

class Participant
	: public IdObject<Participant>
{
private:
	class StatusHandler;
public:
	Participant(const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port);
	~Participant();

	void kick(const string& reason);
private:
	shared_ptr<net::ConnectionPort> mPort;
	std::unique_ptr<StatusHandler> mHandler;
	shared_ptr<EventQueue> mEventQueue;
};


}



#endif