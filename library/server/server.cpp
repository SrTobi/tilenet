#include "includes.hpp"
#include "server/server.hpp"
#include "server/event_queue.hpp"

#include "server/participant.hpp"

namespace srv {

	


Acceptor::Acceptor()
{
}

Acceptor::~Acceptor()
{
}



Server::Server(const TNSVRCONFIG* init)
	: mEvents(std::make_shared<EventQueue>())
	, mServerName(init->name)
	, mServerInfo(init->info)
	, mPackageName(init->pkg)
	, mPackageInterface(init->pkgi)
{

}

Server::~Server()
{

}

void Server::destroy()
{
	NOT_IMPLEMENTED();
}

shared_ptr<TilenetObject> Server::clone()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Server can not be cloned!"));
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

	return participant;
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