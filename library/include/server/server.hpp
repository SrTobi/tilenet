#pragma once
#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <stack>
#include <vector>

#include "settings.hpp"
#include "utils/tilenet_object.hpp"

namespace srv {

class EventQueue;
class Participant;
class ConnectionPort;

struct Acceptor 
	: public TilenetObject
{
public:
	virtual void start() = 0;
};

class Server
	: public TilenetObject
{
public:
	Server(const TNSVRCONFIG* init);
	~Server();

	virtual override void destroy();
	virtual override shared_ptr<TilenetObject> clone();

	void addAcceptor(const std::shared_ptr<Acceptor>& acceptor);
	std::shared_ptr<Participant> addParticipant(const shared_ptr<ConnectionPort>& conport);
	bool fetchNextEvent(TNEVENT* dest, size_t* timeout);


private:
	std::shared_ptr<EventQueue> mEvents;
	std::list<std::shared_ptr<Acceptor>> mAcceptors;
};



}



#endif