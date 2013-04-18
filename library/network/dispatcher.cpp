#include "includes.hpp"
#include "network/dispatcher.hpp"



namespace net {

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::dispatch(const shared_ptr<Message>& msg) const
{
	auto it = mHandlers.find(msg->id());

	if(it == mHandlers.end())
	{
		// No handler found
		NOT_IMPLEMENTED();
	}

	it->second(msg);
}



}