#pragma once
#ifndef _COM_HANDLER_HPP
#define _COM_HANDLER_HPP


#include "settings.hpp"

namespace net {
	class Message;
}

namespace client {
namespace com {


class ComHandler
	: public std::enable_shared_from_this<ComHandler>
{
public:
	virtual shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>&) = 0;
};




}
}




#endif