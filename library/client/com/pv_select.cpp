#include "includes.hpp"
#include "client/com/pv_select.hpp"
#include "client/com_interface.hpp"

// Include all versions
#include "client/com/v1.0/v1_0.hpp"

namespace client {
namespace com {

ProtocolVersionSelect::ProtocolVersionSelect(ClientApp& app, const shared_ptr<net::ConnectionPort>& port)
	: mApp(app)
	, mPort(port)
{
	// add versions
	add<com::v1_0::HandshakeHandler>(com::v1_0::version);

	mDispatcher.add(&ProtocolVersionSelect::handleHandshake, this);
}

ProtocolVersionSelect::~ProtocolVersionSelect()
{
}


OVERRIDE shared_ptr<ComHandler> ProtocolVersionSelect::handleMessage(const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return mSelectedVersion;
}

void ProtocolVersionSelect::handleHandshake( proto::ComInitializing_ProtocolVersion& p )
{
	Log(L"client-com").info() << L"Server wants to use protocol version " << PROTOCOL_MAJOR_VERSION(p.version) << L"." << PROTOCOL_MINOR_VERSION(p.version);

	auto it = mFactories.find(p.version);

	if(it == mFactories.end())
	{
		// Version was not found!!!
		NOT_IMPLEMENTED();
	}

	mSelectedVersion = it->second->create(mApp, mPort);
}

shared_ptr<ComInterface> ProtocolVersionSelect::getComInterface()
{
	return nullptr;
}




}
}