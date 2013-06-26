#include "includes.hpp"
#include "server/server.hpp"
#include "server/event_queue.hpp"

#include "server/participant.hpp"

namespace srv {

	


Acceptor::Acceptor(const shared_ptr<Server>& server)
	: mServer(server)
{
}

Acceptor::~Acceptor()
{
}

const shared_ptr<Server>& Acceptor::server() const
{
	return mServer;
}




Server::Server(const TNSVRCONFIG* init)
	: mEvents(std::make_shared<EventQueue>())
	, mServerName(init->name)
	, mServerInfo(init->info)
	, mPackageName(init->pkg)
	, mPackageInterface(init->pkgi)
	, mNumParticipants(0)
{

}

Server::~Server()
{

}

void Server::destroy()
{
	if(mNumParticipants > 0 || mNumAcceptors > 0)
	{
		// Do not destroy if participants are connected to the server
		NOT_IMPLEMENTED();
	}
}


bool Server::fetchNextEvent( TNEVENT* dest, size_t* timeout )
{
	if(timeout)
	{
		std::chrono::milliseconds timeout_in_milliseconds(*timeout);
		bool success = mEvents->pop(dest, timeout_in_milliseconds);
		*timeout = static_cast<std::size_t>(timeout_in_milliseconds.count());
		return success;
	}else{
		mEvents->pop(dest);
		return true;
	}
}

std::shared_ptr<Participant> Server::addParticipant( const shared_ptr<net::ConnectionPort>& conport )
{
	shared_ptr<Participant> participant = Participant::Create(mEvents, conport, self<Server>());
	++mNumParticipants;
	return participant;
}


void Server::removeParticipant( const std::shared_ptr<Participant>& p )
{
	--mNumParticipants;
}


void Server::addAcceptor()
{
	++mNumAcceptors;
}

void Server::removeAcceptor()
{
	--mNumAcceptors;
}



std::shared_ptr<EventQueue> Server::eventQueue() const
{
	return mEvents;
}

const string& Server::serverName() const
{
	return mServerName;
}

const string& Server::serverInfo() const
{
	return mServerInfo;
}

const string& Server::packageName() const
{
	return mPackageName;
}

const string& Server::packageInterface() const
{
	return mPackageInterface;
}












}