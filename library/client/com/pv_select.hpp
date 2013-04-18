#pragma once
#ifndef _PV_SELECT_HPP
#define _PV_SELECT_HPP

#include <unordered_map>

#include "settings.hpp"
#include "com_handler.hpp"
#include "network/protocol.hpp"
#include "network/dispatcher.hpp"

namespace net {
class ConnectionPort;
}

namespace client {

class ClientApp;

namespace com {

class ComHandlerFactory
{
public:
	virtual shared_ptr<ComHandler> create(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>&) = 0;
};

template<typename Handler>
class ComHandlerFactorySpecialization
	: public ComHandlerFactory
{
public:
	virtual OVERRIDE shared_ptr<ComHandler> create(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port)
	{
		return shared_ptr<ComHandler>(new Handler(app, port));
	}
};

class ProtocolVersionSelect
	: public ComHandler
{
public:
	ProtocolVersionSelect(const shared_ptr<ClientApp>& app, const shared_ptr<net::ConnectionPort>& port);
	~ProtocolVersionSelect();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>&);
private:
	template<typename Handler>
	void add(version_type version)
	{
		mFactories.emplace(version, std::unique_ptr<ComHandlerFactory>(new ComHandlerFactorySpecialization<Handler>()));
	}

	void handleHandshake(const proto::ComInitializing_ProtocolVersion& p);

private:
	shared_ptr<ClientApp> mApp;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<ComHandler> mSelectedVersion;
	
	std::unordered_map<version_type, std::unique_ptr<ComHandlerFactory>> mFactories;

	net::Dispatcher mDispatcher;
};

}
}






#endif