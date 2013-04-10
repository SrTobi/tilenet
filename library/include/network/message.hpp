#pragma once
#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <strstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/noncopyable.hpp>
#include "settings.hpp"

namespace proto {
	template<MsgId Id>
	struct MsgFormat;
}

namespace net {



class Message
	: public boost::noncopyable
{
public:
	Message(const MsgId id, const std::vector<byte>&& msg);
	~Message();


	const std::vector<byte>& buffer() const;
	MsgId id() const;

private:
	MsgId				mId;
	std::vector<byte>	mBuffer;
};



template<MsgId Id>
shared_ptr<Message> make_message(const proto::MsgFormat<Id>& msg)
{
	std::stringstream ss;
	boost::archive::text_oarchive archive(ss);
	archive << Id << msg;

	std::string buf = ss.str();

	std::vector<byte> buffer(buf.begin(), buf.end());

	return shared_ptr<Message>(new Message(Id, std::move(buffer)));
}



template<MsgId Id>
void extract_message(const shared_ptr<Message>& msg, proto::MsgFormat<Id>& dest)
{
	std::stringstream ss(std::string(msg->buffer().begin(), msg->end()));
	boost::archive::text_iarchive archive(ss);
	MsgId extrId;
	archive >> extrId;

	tnAssert(extrId == Id);
	archive >> dest;
}



}


#endif