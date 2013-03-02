#include "includes.h"
#include "network/network.h"

namespace net{

	
bool send(const socket_ptr& socket, const OBuffer::Map& map)
{
	return send(*socket, map);
}


bool send(const socket_ptr& socket, const OBuffer& buf)
{
	return send(*socket, buf);
}


bool send(asio::tcp::socket& socket, const OBuffer::Map& map)
{
	OBuffer collection(net::msg::Collection);
	
	Protocol<net::msg::Collection>::Inject(collection, map);

	return send(socket, collection);
}

bool send(asio::tcp::socket& socket, const OBuffer& buf)
{
	error_code err;
	std::vector<asio::const_buffer> sendData;
	sendData.reserve(3);

	sf::Uint8 sendByte = (buf.getDataSize() > UCHAR_MAX ? 0 : buf.getDataSize());
	
	sendData.push_back(asio::buffer(static_cast<const void*>(&sendByte), sizeof(sendByte)));

	if(!sendByte)
	{
		sf::Uint32 size = htonl(buf.getDataSize());
		sendData.push_back(asio::buffer(static_cast<const void*>(&size), sizeof(size)));
	}


	sendData.push_back(buf.buffer());

	asio::write(socket, sendData, err);

	return !err;
}

/*
void NetPack::extract(sf::Packet& Packet)
{

}

void NetPack::checkPos(const Point& pos)
{
	if(!tool::between(pos.x, -1000, 1000)
		|| !tool::between(pos.y, -1000, 1000))
	{
		mNetId |= ErrorBit;
	}
}

void NetPack::checkSize(const Rect& size)
{
	if(!tool::between(size.w, 1, 1000)
		|| !tool::between(size.h, 1, 1000))
	{
		mNetId |= ErrorBit;
	}
}

*/










}


