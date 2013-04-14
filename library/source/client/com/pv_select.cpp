#include "includes.hpp"
#include "client/com/pv_select.hpp"

// Include all versions
#include "client/com/v1.0/v1_0.hpp"

namespace client {
namespace com {

ProtocolVersionSelect::ProtocolVersionSelect(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port)
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

void ProtocolVersionSelect::handleHandshake( const proto::ComInitializing_ProtocolVersion& p )
{
	auto it = mFactories.find(p.version);

	if(it == mFactories.end())
	{
		// Version was not found!!!
		NOT_IMPLEMENTED();
	}

	mSelectedVersion = it->second->create(mApp, mPort);
}



}
}