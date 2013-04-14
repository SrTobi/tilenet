#include "includes.hpp"
#include "server/participant.hpp"
#include "server/server.hpp"

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
		{
			proto::to_client::Handshake_P1_ProtocolVersion handshake;
			handshake.version = proto::protocol_version;

			port()->send(net::make_message(handshake));
		}

		{
			proto::to_client::Handshake_P2_ServerInformation handshake2;

			handshake2.server_name = p->server()->serverName();
			handshake2.server_info = p->server()->serverInfo();
			handshake2.package_name = p->server()->packageName();
			handshake2.package_interface = p->server()->packageInterface();

			port()->send(net::make_message(handshake2));
		}
	}

	void onMessage(const shared_ptr<net::Message>& msg)
	{
		mDispatcher.dispatch(msg);
	}

	void onDisconnect()
	{
		NOT_IMPLEMENTED();
	}

	net::Dispatcher mDispatcher;
};



Participant::Participant( const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<Server>& server)
	: mPort(port)
	, mEventQueue(eventQueue)
	, mServer(server)
{
}

Participant::~Participant()
{
	NOT_IMPLEMENTED();
}

void Participant::init()
{
	using namespace std::placeholders;

	mPort->setDisconnectHandler(std::bind(&Participant::handleDisconnect, shared_from_this()));
	mPort->setHandler(std::bind(&Participant::handleMessage, shared_from_this(), _1));

	mHandler.reset(new HandshakeStatusHandler(this));
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

shared_ptr<Participant> Participant::Create( const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<Server>& server)
{
	shared_ptr<Participant> p(new Participant(eventQueue, port, server));
	Register(p);
	p->init();

	return p;
}

const shared_ptr<Server>& Participant::server() const
{
	return mServer;
}

}