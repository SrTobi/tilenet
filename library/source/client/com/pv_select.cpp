#include "includes.hpp"
#include "client/com/pv_select.hpp"

namespace client {
namespace com {

ProtocolVersionSelect::ProtocolVersionSelect(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port)
	: mApp(app)
	, mPort(port)
{
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

void ProtocolVersionSelect::handleHandshake( const proto::to_client::Handshake_P1_ProtocolVersion& p )
{
	NOT_IMPLEMENTED();
}



}
}