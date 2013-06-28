#include "includes.hpp"

#include "listen_acceptor.hpp"
#include "server/service.hpp"

#include "network/socket_connection_port.hpp"

namespace srv {

srv::ListenAcceptor::ListenAcceptor(const shared_ptr<Server>& server, port_type port, uint32 maxConnections )
	: Acceptor(server)
	, mPort(port)
	, mMaxConnections(maxConnections)
	, mAcceptor(Service::Inst())
{
}

srv::ListenAcceptor::~ListenAcceptor()
{
}

OVERRIDE void ListenAcceptor::init()
{
	Acceptor::init();

	using namespace boost::asio::ip;

	tcp::endpoint endpoint(tcp::v6(), mPort);
	mAcceptor.open(tcp::v6());
	boost::system::error_code err;

	mAcceptor.bind(endpoint, err);

	if(err)
	{
		BOOST_THROW_EXCEPTION(excp::NetworkException() << excp::InfoWhat(lexical_convert<string>(err.message())));
	}

	mAcceptor.listen(mMaxConnections, err);

	if(err)
	{
		BOOST_THROW_EXCEPTION(excp::NetworkException() << excp::InfoWhat(lexical_convert<string>(err.message())));
	}
}



OVERRIDE void srv::ListenAcceptor::start()
{
	startAsyncAccept();
}

OVERRIDE void srv::ListenAcceptor::stop()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void srv::ListenAcceptor::destroy()
{
	mAcceptor.cancel();
}



void ListenAcceptor::startAsyncAccept()
{
	using namespace boost::asio::ip;

	socket_ptr sock = std::make_shared<tcp::socket>(Service::Inst());
	mAcceptor.async_accept(*sock,
							std::bind(&ListenAcceptor::handleAsyncAccept, self<ListenAcceptor>(),
										sock, std::placeholders::_1)
								);
}

void ListenAcceptor::handleAsyncAccept(const socket_ptr& socket, const boost::system::error_code& err )
{
	if(err)
		return;

	{
		server()->addParticipant(net::SocketConnectionPort::Create(Service::Inst(), socket));
	}

	startAsyncAccept();
}


}