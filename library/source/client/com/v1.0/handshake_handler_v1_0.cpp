#include "includes.hpp"
#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "client/com/v1.0/handshake_handler_v1_0.hpp"


namespace client {
namespace com {
namespace v1_0 {


HandshakeHandler::HandshakeHandler(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port)
	: mApp(app)
	, mPort(port)
{
	tnAssert(app);
	tnAssert(port);

	mDispatcher.add(&HandshakeHandler::handleHandshake, this);
}

HandshakeHandler::~HandshakeHandler()
{

}

OVERRIDE shared_ptr<ComHandler> HandshakeHandler::handleMessage( const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return mNextHandler;
}

void HandshakeHandler::handleHandshake( const proto::v1_0::to_client::Handshake_P2_ServerInformation& handshake )
{
	proto::v1_0::to_srv::Handshake_P3_Confirmation confirmation;

	confirmation.accept_handshake = true;
	mPort->send(net::make_message(confirmation));


}


}}}