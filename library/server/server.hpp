#pragma once
#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <stack>
#include <vector>
#include <list>

#include <atomic>
#include "settings.hpp"
#include "utils/tilenet_object.hpp"


namespace net {
	class ConnectionPort;
}


namespace srv {

class EventQueue;
class Participant;
class Server;

struct Acceptor 
	: public TilenetObject
{
public:
	Acceptor(const shared_ptr<Server>& server);
	virtual ~Acceptor();

	virtual OVERRIDE void destroy() = 0;

	virtual void start() = 0;
	virtual void stop() = 0;

	const shared_ptr<Server>& server() const;
public:
	shared_ptr<Server> mServer;
};

class Server
	: public TilenetObject
{
public:
	Server(const TNSVRCONFIG* init);
	~Server();

	virtual OVERRIDE void destroy();

	std::shared_ptr<Participant> addParticipant(const shared_ptr<net::ConnectionPort>& conport);
	void removeParticipant(const std::shared_ptr<Participant>& p);
	void addAcceptor();
	void removeAcceptor();

	bool fetchNextEvent(TNEVENT* dest, size_t* timeout);

	std::shared_ptr<EventQueue> eventQueue() const;

	const string& serverName() const;
	const string& serverInfo() const;
	const string& packageName() const;
	const string& packageInterface() const;
private:
	const string mServerName;
	const string mServerInfo;
	const string mPackageName;
	const string mPackageInterface;

	std::atomic<unsigned int> mNumParticipants;
	std::atomic<unsigned int> mNumAcceptors;

	std::shared_ptr<EventQueue> mEvents;
	std::list<std::shared_ptr<Acceptor>> mAcceptors;
};



}



#endif