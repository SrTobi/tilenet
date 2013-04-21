#include "includes.hpp"
#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "handshake_handler_v1_0.hpp"
#include "main_com_handler_v1_0.hpp"


namespace client {
namespace com {
namespace v1_0 {


HandshakeHandler::HandshakeHandler(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port)
	: mApp(app)
	, mPort(port)
{
	tnAssert(app);
	tnAssert(port);

	mDispatcher.add(&HandshakeHandler::handleServerInformation, this);
	mDispatcher.add(&HandshakeHandler::handleAccessGranted, this);
}

HandshakeHandler::~HandshakeHandler()
{

}

OVERRIDE shared_ptr<ComHandler> HandshakeHandler::handleMessage( const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return mNextHandler;
}

void HandshakeHandler::handleServerInformation( const proto::v1_0::to_client::Handshake_P2_ServerInformation& handshake )
{
	Log(L"client-com").info()	<< L"Server information:"
								<< L"\nName:      " << handshake.server_name
								<< L"\nInfo:      " << handshake.server_info
								<< L"\nPackage:   " << handshake.package_name
								<< L"\nInterface: " << handshake.package_interface;

	proto::v1_0::to_srv::Handshake_P3_accessrequest confirmation;

	confirmation.accept_handshake = true;
	mPort->send(net::make_message(confirmation));


}

void HandshakeHandler::handleAccessGranted( const proto::v1_0::to_client::Handshake_P4_AcceptesGranted& handshake )
{
	// Yea, we 
	mNextHandler.reset(new MainComHandler(mApp, mPort));
}



}}}