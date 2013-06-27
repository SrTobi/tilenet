#include "includes.hpp"
#include "socket_connection_port.hpp"

#include "network/message.hpp"



namespace net {


SocketConnectionPort::SocketConnectionPort(boost::asio::io_service& service, const socket_ptr& socket)
	: mService(service)
	, mSocket(socket)
{
}

SocketConnectionPort::~SocketConnectionPort()
{
	SocketConnectionPort::onMakeDisconnect();
}

OVERRIDE void SocketConnectionPort::onMakeDisconnect()
{
	if(isConnected())
	{
		mSocket.reset();
		mService.post(std::bind(&SocketConnectionPort::handleDisconnect, shared_from_this()));
	}
}


OVERRIDE void SocketConnectionPort::send( const std::shared_ptr<Message>& msg )
{
	namespace asio = boost::asio;

	msgid_type msgid = msg->id();
	uint8 sizeByte = 0;
	uint32 sizeStorage = 0;
	std::vector<asio::const_buffer> buffers;


	buffers.push_back(asio::buffer(&sizeByte, sizeof(sizeByte)));
	if(msg->size() > 255)
	{
		static_assert(sizeof(msgid_type) == 1, "You must implement network byte conversion!!!");
		sizeStorage = htonl(msg->buffer().size());
		buffers.push_back(asio::buffer(&sizeStorage, sizeof(sizeStorage)));

	}else{
		static_assert(sizeof(msgid_type) == 1, "You must implement network byte conversion!!!");
		sizeByte = msg->buffer().size();
	}


	buffers.push_back(asio::buffer(&msgid, sizeof(msgid)));
	buffers.push_back(asio::buffer(msg->buffer()));


	boost::system::error_code err;

	mSocket->send(buffers, 0, err);

	if(err)
	{
		IMPLEMENTATION_TODO(L"handle error");
	}
}


bool SocketConnectionPort::isConnected() const
{
	return mSocket != nullptr;
}

void SocketConnectionPort::startReceive()
{
	namespace asio = boost::asio;
	using namespace std::placeholders;


	assert(mSocket.get());
	mDataSize = 0;
	mDataSizeByte = 0;

	asio::async_read(*mSocket,
			asio::buffer(&mDataSizeByte, sizeof(mDataSizeByte)),
			std::bind(&SocketConnectionPort::handlePackageHeader, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));
}


void SocketConnectionPort::startBodyReceive()
{
	namespace asio = boost::asio;
	using namespace std::placeholders;

	tnAssert(mDataSize);

	// prepare buffer
	mDataBuffer.clear();
	mDataBuffer.resize(mDataSize);

	// build receive buffer
	static_assert(sizeof(msgid_type) == 1, "You must implement network byte conversion!!!");
	mCombinedBuffer.clear();
	mCombinedBuffer.push_back(boost::asio::buffer(&mMsgId, sizeof(mMsgId)));
	mCombinedBuffer.push_back(boost::asio::buffer(mDataBuffer));

	asio::async_read(*mSocket, mCombinedBuffer,
		std::bind(&SocketConnectionPort::handlePackageBody, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));
}


void SocketConnectionPort::handlePackageHeader( const error_code& err )
{
	namespace asio = boost::asio;
	using namespace std::placeholders;

	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();

	}else if(mDataSizeByte)
	{
		// the message is small enough to fit into a byte
		// set the byte as length
		mDataSize = mDataSizeByte;

		// read the body
		startBodyReceive();

	}else{
		// the message is larger than 255 byte. handle it via another method
		asio::async_read(*mSocket,
			asio::buffer(&mDataSize, sizeof(mDataSize)),
			std::bind(&SocketConnectionPort::handleExtendedPackageHeader, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));

	}
}


void SocketConnectionPort::handleExtendedPackageHeader( const error_code& err )
{
	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();
	}

	// convert byte
	mDataSize = ntohl(mDataSize);

	if(mDataSize == 0)
	{
		IMPLEMENTATION_TODO(L"implement better handling");
		disconnect();
		return;
	}

	startBodyReceive();
}


void SocketConnectionPort::handlePackageBody( const error_code& err )
{
	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();
	}

	static_assert(sizeof(msgid_type) == 1, "You must implement network byte conversion!!!");
	auto msg = std::make_shared<Message>(mMsgId, std::move(mDataBuffer));

	handleReceive(msg);
	startReceive();
}

shared_ptr<SocketConnectionPort> SocketConnectionPort::Create( boost::asio::io_service& service, const socket_ptr& socket )
{
	shared_ptr<SocketConnectionPort> port(new SocketConnectionPort(service, socket));
	port->startReceive();
	return port;
}



}