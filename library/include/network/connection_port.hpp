#pragma once
#ifndef _CONNECTION_PORT_HPP
#define _CONNECTION_PORT_HPP


#include "settings.hpp"



namespace net {

class Message;


class ConnectionPort
{
public:
	virtual void send(const std::shared_ptr<Message>& msg) = 0;
	virtual void setHandler(const std::function<void(const std::shared_ptr<Message>&)> func) = 0;
};


}


#endif