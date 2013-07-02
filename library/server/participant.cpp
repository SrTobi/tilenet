#include "includes.hpp"

#include "participant.hpp"
#include "server.hpp"
#include "service.hpp"
#include "event_queue.hpp"
#include "std_tileset.hpp"
#include "layer_link_manager.hpp"

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


class Participant::MainStatusHandler
	: public StatusHandler
{
public:
	MainStatusHandler(Participant* p)
		: StatusHandler(p)
	{
		mDispatcher.add(&MainStatusHandler::handleRequestStdTileName, this);
		mDispatcher.add(&MainStatusHandler::handleKeyEvent, this);
		mDispatcher.add(&MainStatusHandler::handleTxtEvent, this);
	}


private:
	shared_ptr<StatusHandler> onMessage(const shared_ptr<net::Message>& msg)
	{
		mDispatcher.dispatch(msg);
		return mNextHandler;
	}

	void onDisconnect()
	{

		// Send event
		{
			TNEVENT event;
			event.type = TNEV_DISCONNECT;
			event.participant = participant()->id();

			equeue()->push(event);
		}
	}


	void handleRequestStdTileName(proto::curv::to_srv::Request_StdTileName& req)
	{
		try {
			const string& tilename = StdTileset::Inst().getTileName(req.tileId);

			{
				proto::curv::to_client::Answer_StdTileNameRequest answ;

				answ.tileId = req.tileId;
				answ.tileName = tilename;

				port()->send(net::make_message(answ));
			}
		}catch(const excp::BadIdException& e)
		{
			throw (e << excp::SVFactor(4.0f));
		}
	}

	void handleKeyEvent(proto::curv::to_srv::Control_KeyEvent& e)
	{
		TNEVENT event;
		event.participant = participant()->id();
		event.data.keyevent = e.data;

		if(!proto::curv::KeyMapper::Inst().isKeycode(e.data.key))
		{
			BOOST_THROW_EXCEPTION(excp::ProtocolException() << excp::BadArgument(L"keyevent.key") << excp::SVFactor(0.3f) << excp::InfoWhat(L"Unknown key!"));
		}


		switch(e.type)
		{
		case proto::curv::to_srv::Control_KeyEvent::Evt_KeyDown:
			event.type = TNEV_KEYDOWN;
			break;
		case proto::curv::to_srv::Control_KeyEvent::Evt_KeyUp:
			event.type = TNEV_KEYUP;
			break;
		default:
			BOOST_THROW_EXCEPTION(excp::ProtocolException() << excp::BadArgument(L"keyevent.type") << excp::SVFactor(0.3f) << excp::InfoWhat(L"Unknown key action!"));
		}

		equeue()->push(event);
	}

	void handleTxtEvent(proto::curv::to_srv::Control_TxtEvent& e)
	{
		TNEVENT event;
		event.type = TNEV_TXT;
		event.participant = participant()->id();
		event.data.txtevent = e.data;

		equeue()->push(event);
	}

private:
	shared_ptr<StatusHandler> mNextHandler;
	net::Dispatcher mDispatcher;
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


private:
	shared_ptr<StatusHandler> onMessage(const shared_ptr<net::Message>& msg)
	{
		mDispatcher.dispatch(msg);
		return mNextHandler;
	}

	void onDisconnect()
	{
		// Hmm... maybe he has changed his mind...
		/* Do nothing */
	}

	void handleHandshakeConfirmation(proto::curv::to_srv::Handshake_P3_accessrequest& confirmation)
	{
		if(!confirmation.accept_handshake)
		{
			// Ok... they don't want to be our participant
			participant()->kick(L"No handshake accept!");
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
		
		mNextHandler = std::make_shared<MainStatusHandler>(participant());
	}


private:
	shared_ptr<StatusHandler> mNextHandler;
	net::Dispatcher mDispatcher;
};


Participant::Participant( const shared_ptr<EventQueue>& eventQueue, const shared_ptr<net::ConnectionPort>& port, const shared_ptr<Server>& server)
	: mPort(port)
	, mEventQueue(eventQueue)
	, mServer(server)
	, mSecurityViolationSeverity(static_cast<float>(SecurityViolationKindness))
{
}

Participant::~Participant()
{

}

void Participant::init()
{
	using namespace std::placeholders;


	auto& llm = LayerLinkManager::Inst();
	llm.registerParticipant(shared_from_this());


	mPort->setHandler(shared_from_this());
	mPort->bind(shared_from_this());

	mHandler = std::make_shared<HandshakeStatusHandler>(this);
}

void Participant::kick( const string& reason )
{
	{
		proto::curv::to_client::Kick_Reason k;
		k.reason = reason;
		mPort->send(net::make_message(k));
	}

	mPort->disconnect();
}

void Participant::attachLayer( const shared_ptr<Layer>& layer )
{
	shared_ptr<job::AttachLayerJob> job = std::make_shared<job::AttachLayerJob>(shared_from_this(), layer);
	Service::Inst().enqueJob(job);
}

void Participant::onReceive( const shared_ptr<net::Message>& msg )
{
	Log log(L"participant");
	try {
		shared_ptr<StatusHandler> handler = mHandler->onMessage(msg);

		if(handler)
			mHandler = handler;
	} catch(const excp::ExceptionBase& e)
	{
		log.error() << lexical_convert<string>(boost::diagnostic_information(e));

		auto* svf = boost::get_error_info<excp::SVFactor>(e);

		if(svf)
			mSecurityViolationSeverity -= *svf;

		if(!svf || mSecurityViolationSeverity <= 0.0f)
		{
			// Ok we had too many errors... disconnect!
			kick(L"Too many security violations!");
		}


	} catch(...)
	{
		log.error() << lexical_convert<string>(boost::current_exception_diagnostic_information());
		log.error() << L"Unknown error disconnect participant[" << id() <<  L"]!";
		kick(L"Heavy security violation!!!");
	}
}

void Participant::onDisconnect()
{
	{
		auto& llm = LayerLinkManager::Inst();

		llm.unregisterParticipant(shared_from_this());

		mServer->removeParticipant(shared_from_this());
	}

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
