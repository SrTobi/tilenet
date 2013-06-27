#include "includes.hpp"
#include "network/message.hpp"



namespace net {







Message::Message(const msgid_type id, std::vector<byte>&& msg)
	: mBuffer(std::move(msg))
	, mId(id)
{
}

Message::~Message()
{
}

const std::vector<byte>& Message::buffer() const
{
	return mBuffer;
}

msgid_type Message::id() const
{
	return mId;
}

std::size_t Message::size() const
{
	return mBuffer.size();
}


}