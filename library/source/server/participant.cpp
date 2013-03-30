#include "includes.hpp"
#include "server/participant.hpp"


namespace srv {

class Participant::StatusHandler
{

};




Participant::Participant( const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port )
{
	NOT_IMPLEMENTED();
}

Participant::~Participant()
{
	NOT_IMPLEMENTED();
}

void Participant::kick( const string& reason )
{
	NOT_IMPLEMENTED();
}

}