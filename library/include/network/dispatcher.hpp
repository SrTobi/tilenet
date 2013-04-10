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
	template<msgid_type Id>
	struct DispatchHandler
	{
		typedef proto::MsgFormat<Id> msg_type;
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

	template<msgid_type Id, typename FuncContext, typename CallContext>
	void add(void (FuncContext::*func)(const proto::MsgFormat<Id>&), CallContext context)
	{
		using namespace std::placeholders;

		//add(std::bind(func, context, _1));
		shared_ptr<DispatchHandler<Id>> handler(new DispatchHandler<Id>(std::bind(func, context, _1)));
		mHandlers.emplace(Id, std::bind(&DispatchHandler<Id>::handle, handler, _1));
	}

	/*template<msgid_type Id>
	void add(const std::function<void(const proto::MsgFormat<Id>&)>& func)
	{
		using namespace std::placeholders;
		shared_ptr<DispatchHandler<Id>> handler(new DispatchHandler<Id>(func));
		mHandlers.emplace(Id, std::bind(&DispatchHandler<Id>::handle, handler, _1));
	}*/

	void dispatch(const shared_ptr<Message>& msg) const;

private:
	std::unordered_map<msgid_type, std::function<void(const shared_ptr<Message>&)> > mHandlers;
};


}






#endif