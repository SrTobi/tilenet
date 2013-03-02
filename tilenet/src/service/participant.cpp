#include "includes.h"
#include "service/participant.h"
#include "service/netservice.h"

#include "server/display.h"
#include "server/server.h"

#include "network/network.h"

namespace service {



Participant::Participant(socket_ptr socket, NetService* service, uint id)
	:	net::NetReceiver(socket),
		mId(id),
		mService(service)
{
	SLOG(Construct, L"Construct participant...");
}

void Participant::holdRef()
{
	syncronize<decltype(mSelfAlloc)> selfAlloc(mSelfAlloc);
	selfAlloc->push(shared_from_derived_this());
}

bool Participant::releaseMe()
{
	syncronize<decltype(mSelfAlloc)> selfAlloc(mSelfAlloc);
	bool canPop = selfAlloc->size() != 0;
	if(canPop)
		selfAlloc->pop();

	return canPop;
}

Participant::~Participant()
{
}

//@Override
bool Participant::onPackage(net::IBuffer& buffer)
{
	switch(buffer.id())
	{
	case net::msg::Collection:
		NOT_IMPL()
		break;

	case net::msg::ToServer::RequestTileDef:
		{
			net::Protocol<net::msg::ToServer::RequestTileDef> protocol(buffer);
			
			const wstring& name = mService->server()->resolveTileId(protocol.tileId);

			net::OBuffer answer(net::msg::ToClient::SendTileDefinition);
			net::Protocol<net::msg::ToClient::SendTileDefinition>::Inject(answer, protocol.tileId, name);

			net::send(socket(), answer);

		}break;

	case net::msg::ToServer::SendKeyEvent:
		{
			net::Protocol<net::msg::ToServer::SendKeyEvent> protocol(buffer);
			mService->server()->injectEvent_KeyEvent(id(), tn::KeyEvent(protocol.option, protocol.keycode));
		}break;
	case net::msg::Error:
	default:
		SLOG(Error, L"Unknown command from praticipant!");
		break;
	}

	return true;
}
	
//@Override
void Participant::onSocketClose()
{
	shared_ptr<Participant> p = mService->eraseParticipant(mId);
	assert(p.get());
	mService->server()->injectEvent_ParticipantDisconnected(p);
}


//@Override
server::Display* Participant::attachDisplay(tn::Display* display)
{
	server::Display* oldDisplay = detachDisplay();

	if(!display)
		return oldDisplay;

	server::Display* d = dynamic_cast<server::Display*>(display);

	if(!d)
		throw tn::TnException(L"Wrong display type!");

	mDisplay = d->shared_from_this();
	mDisplay->addParticipant(shared_from_derived_this());

	mService->fullDisplayTransmittion_to_partitipant(shared_from_derived_this());

	return oldDisplay;
}

//@Override
server::Display* Participant::detachDisplay()
{
	server::Display* oldDisplay = mDisplay.get();
	mDisplay.reset();
	return oldDisplay;
}

//@Override
server::Display* Participant::getDisplay()
{
	return mDisplay.get();
}
	
//@Override
bool Participant::kick()
{
	socket()->close();

	return true;
}

uint Participant::id() const
{
	return mId;
}


}