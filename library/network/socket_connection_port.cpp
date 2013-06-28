#include "includes.hpp"
#include "socket_connection_port.hpp"

#include "network/message.hpp"



namespace net {

int debug_file_index = 0;


SocketConnectionPort::SocketConnectionPort(boost::asio::io_service& service, const socket_ptr& socket)
	: mService(service)
	, mSocket(socket)
	, mStatus(Idle)
{
	tnAssert(mSocket);

	
	std::string name = "socket_" + boost::lexical_cast<std::string>(debug_file_index++) + ".txt";
	mDebugFile.open(name.c_str());
}

SocketConnectionPort::~SocketConnectionPort()
{
	SocketConnectionPort::onMakeDisconnect();
}

OVERRIDE void SocketConnectionPort::onMakeDisconnect()
{
	std::lock_guard<std::mutex> guard(mMutex);
	if(isConnected())
	{
		boost::system::error_code err;
		mSocket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, err);
		mSocket->close(err);
		mSocket.reset();
		mService.post(std::bind(&SocketConnectionPort::handleDisconnect, shared_from_this()));
	}
}


OVERRIDE void SocketConnectionPort::send( const std::shared_ptr<Message>& msg )
{
	std::lock_guard<std::mutex> guard(mMutex);

	if(!mSocket)
		return;

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

	asio::write(*mSocket, buffers, err);

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

	tnAssert(mStatus == Idle);


	if(!mSocket)
		return;

	mDataSize = 0;
	mDataSizeByte = 0;

	mStatus = AwaitHeader;
	asio::async_read(*mSocket,
			asio::buffer(&mDataSizeByte, sizeof(mDataSizeByte)),
			std::bind(&SocketConnectionPort::handlePackageHeader, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));
}


void SocketConnectionPort::startBodyReceive()
{
	if(!mSocket)
		return;

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

	mStatus = AwaitBody;
	asio::async_read(*mSocket, mCombinedBuffer,
		std::bind(&SocketConnectionPort::handlePackageBody, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));
}


void SocketConnectionPort::handlePackageHeader( const error_code& err )
{
	namespace asio = boost::asio;
	using namespace std::placeholders;

	tnAssert(mStatus == AwaitHeader);
	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();

	}else if(mDataSizeByte)
	{
		mDebugFile << "byte-size(" << (int)mDataSizeByte << "):";

		// the message is small enough to fit into a byte
		// set the byte as length
		mDataSize = mDataSizeByte;

		// read the body
		startBodyReceive();

	}else{
		// the message is larger than 255 byte. handle it via another method
		mStatus = AwaitExtendedHeader;
		asio::async_read(*mSocket,
			asio::buffer(&mDataSize, sizeof(mDataSize)),
			std::bind(&SocketConnectionPort::handleExtendedPackageHeader, std::static_pointer_cast<SocketConnectionPort>(shared_from_this()), _1));

	}
}


void SocketConnectionPort::handleExtendedPackageHeader( const error_code& err )
{
	tnAssert(mStatus == AwaitExtendedHeader);
	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();
		return;
	}

	// convert byte
	mDataSize = ntohl(mDataSize);

	mDebugFile << "size(" << mDataSize << "):";

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
	tnAssert(mStatus == AwaitBody);
	if(err)
	{
		IMPLEMENTATION_TODO("do something intelligent!");
		disconnect();
		return;
	}

	static_assert(sizeof(msgid_type) == 1, "You must implement network byte conversion!!!");
	auto msg = std::make_shared<Message>(mMsgId, std::move(mDataBuffer));

	mStatus = Idle;

	mDebugFile << "[" << (int)mMsgId << "]";
	std::copy(msg->buffer().begin(), msg->buffer().end(), std::ostream_iterator<char>(mDebugFile));
	mDebugFile << std::endl;

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