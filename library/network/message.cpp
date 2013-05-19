#include "includes.hpp"
#include "network/message.hpp"



namespace net {







<<<<<<< HEAD
Message::Message(const msgid_type id, std::vector<byte>&& msg)
	: mBuffer(std::move(msg))
=======
Message::Message(const msgid_type id, const buffer_type&& msg)
	: mBuffer(msg)
>>>>>>> new Serializer integrated.
	, mId(id)
{
}

Message::~Message()
{
}

const Message::buffer_type& Message::buffer() const
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