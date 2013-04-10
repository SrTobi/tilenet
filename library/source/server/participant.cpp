#include "includes.hpp"
#include "server/participant.hpp"

#include "network/protocol.hpp"
#include "network/message.hpp"
#include "network/connection_port.hpp"
#include "network/dispatcher.hpp"

namespace srv {

class Participant::StatusHandler
{
public:
	StatusHandler(Participant* p)
		: mParticipant(p)
		, mPort(p->mPort)
		, mEventQueue(p->mEventQueue)
	{
	}

	virtual void onMessage(const shared_ptr<net::Message>& msg) = 0;
	virtual void onDisconnect() = 0;

	Participant* participant() const { return mParticipant; }
	const shared_ptr<net::ConnectionPort> port() const { return mPort; }
	const shared_ptr<EventQueue> equeue() const { return mEventQueue; }
private:
	Participant*			mParticipant;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<EventQueue> mEventQueue;
};


class Participant::HandshakeStatusHandler
	: public StatusHandler
{
public:
	HandshakeStatusHandler(Participant* p)
		: StatusHandler(p)
	{
		proto::to_client::Handshake_P1_ProtocolVersion handshake;
		handshake.version = proto::protocol_version;

		port()->send(net::make_message(handshake));
	}

	void onMessage(const shared_ptr<net::Message>& msg)
	{
		NOT_IMPLEMENTED();
	}

	void onDisconnect()
	{
		NOT_IMPLEMENTED();
	}
};



Participant::Participant( const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port )
	: mPort(port)
	, mEventQueue(eventQueue)
{
	using namespace std::placeholders;

	mHandler.reset(new HandshakeStatusHandler(this));
	
	mPort->setDisconnectHandler(std::bind(&Participant::handleDisconnect, shared_from_this()));
	mPort->setHandler(std::bind(&Participant::handleMessage, shared_from_this(), _1));
}

Participant::~Participant()
{
	NOT_IMPLEMENTED();
}

void Participant::kick( const string& reason )
{
	NOT_IMPLEMENTED();
}

void Participant::handleMessage( const shared_ptr<net::Message>& msg )
{
	mHandler->onMessage(msg);
}

void Participant::handleDisconnect()
{
	mHandler->onDisconnect();
}

}