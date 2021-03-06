#pragma once
#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <sstream>
#include "settings.hpp"

#include "protocol.hpp"

#include "serialization/serialization.hpp"

namespace net {



class Message
	: public boost::noncopyable
{
public:
	Message(const msgid_type id, std::vector<byte>&& msg);
	~Message();

	std::size_t size() const;
	const std::vector<byte>& buffer() const;
	msgid_type id() const;

private:
	msgid_type				mId;
	std::vector<byte>	mBuffer;
};



template<msgid_type Id, proto::versions::Version V>
shared_ptr<Message> make_message(const proto::MsgFormat<Id, V>& msg)
{
	std::stringstream ss;
	serialization::TextSerializer<> archive(ss);

	archive << std::hex << msg;

	std::string buf = ss.str();

	std::vector<byte> buffer(buf.begin(), buf.end());

	return std::make_shared<Message>(Id, std::move(buffer));
}



template<msgid_type Id, proto::versions::Version V>
void extract_message(const shared_ptr<Message>& msg, proto::MsgFormat<Id, V>& dest)
{
	std::stringstream ss(std::string(msg->buffer().begin(), msg->buffer().end()));
	serialization::TextDeserializer<> archive(ss);

	archive >> std::hex >> dest;
}



}


#endif
