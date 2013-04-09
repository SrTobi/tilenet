#pragma once
#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <stack>
#include <vector>

#include "settings.hpp"
#include "utils/tilenet_object.hpp"


namespace net {
	class ConnectionPort;
}


namespace srv {

class EventQueue;
class Participant;

struct Acceptor 
	: public TilenetObject
{
public:
	Acceptor();
	virtual ~Acceptor();

	virtual OVERRIDE void destroy() = 0;
	virtual OVERRIDE shared_ptr<TilenetObject> clone() = 0;
	OVERRIDE void flush();

	virtual void start() = 0;
	virtual void stop() = 0;
};

class Server
	: public TilenetObject
{
public:
	Server(const TNSVRCONFIG* init);
	~Server();

	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();
	virtual OVERRIDE void flush();

	void addAcceptor(const std::shared_ptr<Acceptor>& acceptor);
	std::shared_ptr<Participant> addParticipant(const shared_ptr<net::ConnectionPort>& conport);
	bool fetchNextEvent(TNEVENT* dest, size_t* timeout);

	std::shared_ptr<EventQueue> eventQueue() const;

private:
	std::shared_ptr<EventQueue> mEvents;
	std::list<std::shared_ptr<Acceptor>> mAcceptors;
};



}



#endif