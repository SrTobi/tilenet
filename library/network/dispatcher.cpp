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
		BOOST_THROW_EXCEPTION(excp::DispatchException() << excp::BadId(msg->id()) << excp::SVFactor(1.0f) << excp::InfoWhat(L"Failed to dispatch the message!"));
	}

	it->second(msg);
}



}