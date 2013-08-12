#pragma once
#ifndef _TILEY_ACCEPTORS_HPP
#define _TILEY_ACCEPTORS_HPP

#include "server.hpp"

namespace tiley {


class LocalAcceptor
	: public TNObject
{
public:
	template<typename Ch, typename Mutex>
	LocalAcceptor(const Server<Ch, Mutex>& srv, bool detached = true)
	{
		if(detached)
			Impl::AddDetachedLocalAcceptor(srv.native_ref());
		else
			reset(Impl::AddLocalAcceptor(srv.native_ref()));
	}

	~LocalAcceptor()
	{
	}
};

class ListenAcceptor
	: public TNObject
{
public:
	typedef unsigned short port_type;
	typedef size_t size_type;
public:

	ListenAcceptor(port_type port, size_type maxCon = 32)
		: mPort(port)
		, mMaxConnections(maxCon)
	{
	}

	template<typename Ch, typename Mutex>
	ListenAcceptor(const Server<Ch, Mutex>& srv, port_type port, size_type maxCon = 32)
		: mPort(port)
		, mMaxConnections(maxCon)
	{
		start(srv);
	}

	~ListenAcceptor()
	{
	}

	template<typename Ch, typename Mutex>
	void start(const Server<Ch, Mutex>& srv)
	{
		assert(!native_ref());
		Impl::AddListenAcceptor(srv.native_ref(), port(), maxConnections());
	}

	void stop()
	{
		reset(nullptr);
	}

	void port(port_type p)
	{
		assert(!native_ref());
		mPort = p;
	}

	port_type port() const
	{
		 return mPort;
	}

	void maxConnections(size_type mc)
	{
		assert(!native_ref());
		mMaxConnections = mc;
	}

	size_type maxConnections() const
	{
		return mMaxConnections;
	}

private:
	port_type mPort;
	size_type mMaxConnections;
};


}




#endif