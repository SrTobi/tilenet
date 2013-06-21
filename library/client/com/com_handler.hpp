#pragma once
#ifndef _COM_HANDLER_HPP
#define _COM_HANDLER_HPP


#include "settings.hpp"

namespace net {
	class Message;
}

namespace client {

class ComInterface;

namespace com {


class ComHandler
	: public std::enable_shared_from_this<ComHandler>
{
public:
	virtual ~ComHandler() {}
	virtual shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>&) = 0;

	virtual shared_ptr<ComInterface> getComInterface() = 0;
};




}
}




#endif