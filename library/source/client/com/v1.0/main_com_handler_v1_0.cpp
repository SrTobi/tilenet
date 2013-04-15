#include "includes.hpp"
#include "client/com/v1.0/main_com_handler_v1_0.hpp"



namespace client {
namespace com {
namespace v1_0 {




MainComHandler::MainComHandler( const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port )
	: mApp(app)
	, mPort(port)
{
}

MainComHandler::~MainComHandler()
{
}

OVERRIDE shared_ptr<ComHandler> MainComHandler::handleMessage(const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return shared_ptr<ComHandler>();
}

}}}