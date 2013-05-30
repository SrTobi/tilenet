#include "includes.hpp"

#include "network/local_connection_port.hpp"


namespace net {


LocalConnectionPort::LocalConnectionPort( boost::asio::io_service& service )
	: mService(service)
{
}

LocalConnectionPort::~LocalConnectionPort()
{
	LocalConnectionPort::onMakeDisconnect();
}

OVERRIDE void LocalConnectionPort::onMakeDisconnect()
{
	shared_ptr<LocalConnectionPort> other = mConnectedPort.lock();

	mConnectedPort.reset();

	if(other)
	{
		other->disconnect();
		mService.post(std::bind(&LocalConnectionPort::handleDisconnect, shared_from_this()));
	}
}


OVERRIDE void LocalConnectionPort::send( const std::shared_ptr<Message>& msg )
{
	shared_ptr<LocalConnectionPort> other = mConnectedPort.lock();

	if(other)
		other->mService.post(std::bind(&LocalConnectionPort::handleReceive, other, msg));
}

std::tuple<shared_ptr<LocalConnectionPort>, shared_ptr<LocalConnectionPort>> LocalConnectionPort::Create( boost::asio::io_service& s1, boost::asio::io_service& s2 )
{
	shared_ptr<LocalConnectionPort> p1(new LocalConnectionPort(s1));
	shared_ptr<LocalConnectionPort> p2(new LocalConnectionPort(s2));

	p1->mConnectedPort = p2;
	p2->mConnectedPort = p1;

	return std::make_tuple(p1, p2);
}


bool LocalConnectionPort::isConnected() const
{
	return bool(mConnectedPort.lock());
}



}