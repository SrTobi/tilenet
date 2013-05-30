#pragma once
#ifndef _CONNECTION_PORT_HPP
#define _CONNECTION_PORT_HPP


#include "settings.hpp"


namespace net {

class Message;


class ConnectionPort
	: public std::enable_shared_from_this<ConnectionPort>
{
public:
	struct Handler
	{
		virtual ~Handler() {}

		virtual void onReceive(const std::shared_ptr<Message>& msg) = 0;
		virtual void onDisconnect() = 0;
	};

	ConnectionPort();
	virtual ~ConnectionPort();

	
	virtual void send(const std::shared_ptr<Message>& msg) = 0;
	virtual bool isConnected() const = 0;

	void disconnect();
	void setHandler(const shared_ptr<Handler>& handler);
	void bind(const shared_ptr<Handler>& binding);
	shared_ptr<Handler> handler() const;
protected:
	virtual void onMakeDisconnect() = 0;
	virtual void onHandlerSet();
	void handleReceive(const std::shared_ptr<Message>& msg) const;
	void handleDisconnect();

private:
	weak_ptr<Handler> mHandler;
	shared_ptr<Handler> mBinding;
};


}


#endif