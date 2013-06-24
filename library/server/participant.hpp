#pragma once
#ifndef _PARTICIPANT_HPP
#define _PARTICIPANT_HPP


#include "settings.hpp"

#include "network/protocol.hpp"
#include "network/connection_port.hpp"

namespace net {
	class ConnectionPort;
	class Message;
}

namespace srv {

class Server;
class EventQueue;
class Layer;


class Participant
	: public std::enable_shared_from_this<Participant>
	, public IdObject<Participant>
	, public net::ConnectionPort::Handler
{
private:
	class StatusHandler;
	class HandshakeStatusHandler;
	class MainStatusHandler;
public:
	~Participant();

	void kick(const string& reason);
	void attachLayer(const shared_ptr<Layer>& layer);

	const shared_ptr<Server>& server() const;
	const shared_ptr<net::ConnectionPort>& port() const;

	static shared_ptr<Participant> Create(const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<Server>& server);
private:
	Participant(const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<Server>& server);

	void init();

	void onReceive(const shared_ptr<net::Message>& msg);
	void onDisconnect();

private:
	const shared_ptr<Server> mServer;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<EventQueue> mEventQueue;
	shared_ptr<StatusHandler> mHandler;
};


}



#endif