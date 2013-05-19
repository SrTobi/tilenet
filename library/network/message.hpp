#pragma once
#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/noncopyable.hpp>
#include "settings.hpp"

#include "protocol.hpp"
#include "protocol_serializer.hpp"
#include "protocol_deserializer.hpp"


namespace net {



class Message
	: public boost::noncopyable
{
public:
	typedef std::vector<char> buffer_type;

	Message(const msgid_type id, buffer_type&& msg);
	~Message();


	std::size_t size() const;
	const buffer_type& buffer() const;
	msgid_type id() const;

private:
	msgid_type				mId;
	buffer_type	mBuffer;
};



template<msgid_type Id, proto::versions::Version V>
shared_ptr<Message> make_message(const proto::MsgFormat<Id, V>& msg)
{
	std::vector<char> buffer;
	{
		ProtocolSerializer<> serializer(buffer);

		msgid_type msgId = Id;
		serializer << msgId << msg;
	}


	return std::make_shared<Message>(Id, std::move(buffer));
}



template<msgid_type Id, proto::versions::Version V>
void extract_message(const shared_ptr<Message>& msg, proto::MsgFormat<Id, V>& dest)
{
	ProtocolDeserializer<>::range_type bufRange(msg->buffer().begin(), msg->buffer().end());
	ProtocolDeserializer<> deserializer(bufRange);

	msgid_type extrId;
	deserializer >> extrId;

	tnAssert(extrId == Id);
	deserializer >> dest;
}



}


#endif
