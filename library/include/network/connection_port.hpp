#pragma once
#ifndef _CONNECTION_PORT_HPP
#define _CONNECTION_PORT_HPP


#include "settings.hpp"



namespace net {

class Message;


class ConnectionPort
{
public:
	typedef std::function<void()> disconnect_handler_type;
	typedef std::function<void(const std::shared_ptr<Message>&)> handler_type;
	ConnectionPort();
	~ConnectionPort();

	virtual void send(const std::shared_ptr<Message>& msg) = 0;
	
	void setHandler(const handler_type& func);
	void setDisconnectHandler(const disconnect_handler_type& func);
	bool hasHandler() const;
	bool hasDisconnectHandler() const;
protected:
	virtual void onHandlerSet();
	void handleReceive(const std::shared_ptr<Message>& msg) const;
	void handleDisconnect() const;

private:
	handler_type mHandler;
	disconnect_handler_type mDisconnectHandler;
};


}


#endif