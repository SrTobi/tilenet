#include "includes.hpp"
#include "network/message.hpp"



namespace net {







Message::Message(const MsgId id, const std::vector<byte>&& msg)
	: mBuffer(msg)
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

MsgId Message::id() const
{
	return mId;
}

}