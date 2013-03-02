#include "includes.h"
#include "service/pending.h"
#include "service/netservice.h"



namespace service {


PendingConnection::PendingConnection(socket_ptr socket, NetService* service)
	:	NetReceiver(socket),
		mService(service)
{
	SLOG(Construct, L"Construct pending...");
}


PendingConnection::~PendingConnection()
{
}

void PendingConnection::sendWelcomeHandshake()
{
	// Handshake senden
	net::OBuffer::Map handshake;
	handshake["subject"] = L"handshake";
	handshake["version"] = boost::lexical_cast<nstring>(version::CurrentVersion);
	handshake["stdpackage"] = tn::helper::ToWideString(mService->config().tilePackageName);
	handshake["interface"] = tn::helper::ToWideString(mService->config().tilePackageInterface);

	net::send(socket(), handshake);
}

//@Oberride
bool PendingConnection::onPackage(net::IBuffer& buffer)
{
	switch(buffer.id())
	{
	case net::msg::Collection:
		{
			net::Protocol<net::msg::Collection> protocol(buffer);
			return handleCollection(protocol.collection);
		}
	default:
		CLOG(Error, L"Invalid request from server!");
		return false;
	};

	return true;
}
	
//@Oberride
void PendingConnection::onSocketClose()
{
	SLOG(Info, L"Participant has disconnected!");
}



bool PendingConnection::handleCollection(net::IBuffer::Map& request)
{
	wstring subject(request["subject"]);

	if(subject == L"handshake_back")
	{
		const TilenetConfig& config = mService->config();
		net::IBuffer::Map answ;

		answ["subject"] = L"answer";

		if(answ["password"] !=  tn::helper::ToWideString(config.serverPassword))
		{
			answ["answer"] = L"invalid_password";
		}else{

			shared_ptr<Participant> newPlayer = mService->createParticipant(socket());
			newPlayer->startReceiving();

			answ["answer"] = L"access_granted";
		}

		net::send(socket(), answ);
		return false;


	}else{
		SVLOG(Warning, L"Unknown request[" << request << L"]");
		return false;
	}

	return true;
}




}
