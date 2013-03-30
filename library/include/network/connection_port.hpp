#pragma once
#ifndef _CONNECTION_PORT_HPP
#define _CONNECTION_PORT_HPP


#include "settings.hpp"



namespace net {

class Message;


class ConnectionPort
{
public:
	class HandlerTicket;
	typedef shared_ptr<HandlerTicket> ticket_type;

	virtual void send(const std::shared_ptr<Message>& msg) = 0;
	virtual ticket_type addHandler(const std::function<void(const std::shared_ptr<Message>&)> func) = 0;
};


}


#endif