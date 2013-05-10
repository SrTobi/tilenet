#include "includes.hpp"

#include "participant.hpp"
#include "server.hpp"
#include "service.hpp"
#include "event_queue.hpp"

#include "network/protocol.hpp"
#include "network/message.hpp"
#include "network/connection_port.hpp"
#include "network/dispatcher.hpp"

#include "jobs/service_job.hpp"
#include "jobs/attach_layer_job.hpp"

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

	virtual shared_ptr<StatusHandler> onMessage(const shared_ptr<net::Message>& msg) = 0;
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
		// Register handlers
		mDispatcher.add(&HandshakeStatusHandler::handleHandshakeConfirmation, this);

		// Send some messages to the participant
		{
			proto::ComInitializing_ProtocolVersion handshake;
			handshake.version = proto::curv::protocol_version;

			port()->send(net::make_message(handshake));
		}

		{
			proto::curv::to_client::Handshake_P2_ServerInformation handshake2;

			handshake2.server_name = p->server()->serverName();
			handshake2.server_info = p->server()->serverInfo();
			handshake2.package_name = p->server()->packageName();
			handshake2.package_interface = p->server()->packageInterface();

			port()->send(net::make_message(handshake2));
		}
	}


	shared_ptr<StatusHandler> onMessage(const shared_ptr<net::Message>& msg)
	{
		mDispatcher.dispatch(msg);
		return mNextHandler;
	}

	void onDisconnect()
	{
		NOT_IMPLEMENTED();
	}

private:
	void handleHandshakeConfirmation(const proto::curv::to_srv::Handshake_P3_accessrequest& confirmation)
	{
		if(!confirmation.accept_handshake)
		{
			// Ok... they don't want to be our participant
			NOT_IMPLEMENTED();
		}

		// accept the participant
		{
			Log(L"Service").info()	<< L"Participant connected!";

			TNEVENT e;
			e.type = TNEV_CONNECT;
			e.participant = participant()->id();
			equeue()->push(e);
		}

		{
			proto::curv::to_client::Handshake_P4_AcceptesGranted granted;
			granted.access_granted = true;

			port()->send(net::make_message(granted));
		}
		
	}


private:
	shared_ptr<StatusHandler> mNextHandler;
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

void Participant::attachLayer( const shared_ptr<Layer>& layer )
{
	mAttachedLayer = layer;
	shared_ptr<job::AttachLayerJob> job(new job::AttachLayerJob(shared_from_this(), layer));
	Service::Inst().enqueJob(job);
}

void Participant::handleMessage( const shared_ptr<net::Message>& msg )
{
	shared_ptr<StatusHandler> handler = mHandler->onMessage(msg);

	if(handler)
		mHandler = handler;
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

const shared_ptr<net::ConnectionPort>& Participant::port() const
{
	return mPort;
}



}