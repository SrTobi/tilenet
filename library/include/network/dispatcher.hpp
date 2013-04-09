#pragma once
#ifndef _DISPATCHER_HPP
#define _DISPATCHER_HPP

#include <unordered_map>

#include "settings.hpp"
#include "protocol.hpp"
#include "message.hpp"

namespace net {

class Message;

class Dispatcher
{
	template<MsgId Id>
	struct DispatchHandler
	{
		typedef MsgFormat<Id> msg_type;
		DispatchHandler(const std::function<void(msg_type&)>& func)
			: mDispatcher(func)
		{
		}

		void handle(const shared_ptr<Message>& msg)
		{
			msg_type ext;
			extract_message(msg, ext);

			mDispatcher(ext);
		}

		std::function<void(msg_type&)> mDispatcher;
	};
public:
	Dispatcher();
	~Dispatcher();

	template<MsgId Id>
	void add(const std::function<void(const MsgFormat<Id>&)>& func)
	{
		shared_ptr<DispatchHandler<Id>> handler(new DispatchHandler<Id>(func));
		mHandlers.emplace(Id, std::bind<void(const shared_ptr<Message>&)>(&DispatchHandler<Id>::handle, handler, _1));
	}

	void dispatch(const shared_ptr<Message>& msg) const;

private:
	std::unordered_map<MsgId, std::function<void(const shared_ptr<Message>&)> > mHandlers;
};


}






#endif