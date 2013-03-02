#include "includes.h"
#include "network/netReceiver.h"


namespace net {

NetReceiver::NetReceiver()
{
}

NetReceiver::NetReceiver(socket_ptr socket)
	: mSocket(socket)
{
}


NetReceiver::~NetReceiver()
{
}


void NetReceiver::setSocket(socket_ptr socket)
{
	mSocket = socket;
}


void NetReceiver::startReceiving()
{
	assert(mSocket.get());
	mDataSize = 0;

	asio::async_read(*mSocket,
			asio::buffer(&mDataSizeByte, sizeof(mDataSizeByte)),
			boost::bind(&NetReceiver::handlePackageHeader, shared_from_this(), asio::placeholders::error));
}


void NetReceiver::startReceivingBody()
{
	assert(mDataSize);

	// buffer vorbereiten
	mBuffer.prepare(mDataSize);

	asio::async_read(*mSocket,
		asio::buffer(mBuffer.getMutableData(), mDataSize),
		boost::bind(&NetReceiver::handlePackageBody, shared_from_this(), asio::placeholders::error));
}

	
void NetReceiver::handlePackageHeader(const error_code& err)
{
	if(err)
	{
		onSocketClose();

	}else if(mDataSizeByte)
	{
		// Die Größe passte in ein byte. jetzt kann direkt das packet gelesen werden 

		// Byte als richtige länge eintragen
		mDataSize = mDataSizeByte;

		// Und body lesen
		startReceivingBody();

	}else{
		// Die Größe passte nicht in ein byte. jetzt muss noch eine 4-byte größe empfangen werden
		asio::async_read(*mSocket,
			asio::buffer(static_cast<void*>(&mDataSize), sizeof(mDataSize)),
			boost::bind(&NetReceiver::handleExtendedPackageHeader, shared_from_this(), asio::placeholders::error));

	}

}

void NetReceiver::handleExtendedPackageHeader(const error_code& err)
{
	if(err)
	{
		onSocketClose();
		NOT_IMPL();
	}

	// Wichtig byte-order korigieren
	mDataSize = ntohl(mDataSize);
	
	startReceivingBody();
}

void NetReceiver::handlePackageBody(const error_code& err)
{
	if(err)
	{
		onSocketClose();
		NOT_IMPL();
	}

	if(onPackage(mBuffer))
		startReceiving();
	else
		onSocketClose();
}



}
