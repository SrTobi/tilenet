#pragma once
#ifndef _TILEY_PANEL_LAYER_HPP
#define _TILEY_PANEL_LAYER_HPP

#include <forward_list>

#include "auto_object.hpp"
#include "layer.hpp"
#include "lockable.hpp"
#include "flushable.hpp"

namespace tiley {

template<typename AttachableType>
class AttachSocket;

template<typename AttachableType>
class Attachable
	: public virtual Lockable
{
	template<typename _AttachableType>
	friend class AttachSocket;
public:
	typedef AttachSocket<AttachableType> attach_socket;
public:
	Attachable()
	{
	}

	virtual ~Attachable()
	{
		detach_all();
	}

	void detach_all()
	{
		Lock lock(*this);

		while (!mAttached.empty())
		{
			mAttached.front()->_detach_silent();
			mAttached.pop_front();
		}
	}

protected:
	void _flush_attached() const
	{
		for (auto* attached : mAttached)
			attached->flush();
	}

private:
	void _attach(attach_socket* socket)
	{
		Lock lock(*this);
		mAttached.push_front(socket);
	}

	void _detach(attach_socket* socket)
	{
		Lock lock(*this);
		mAttached.remove(socket);
	}

private:
	std::forward_list<attach_socket*> mAttached;
};


template<typename AttachableType>
class AttachSocket
	: public virtual Flushable
	, public virtual Lockable
{
	template<typename _AttachableType>
	friend class Attachable;
public:
	typedef AttachableType attachable_type;

public:
	AttachSocket()
		: mAttached(nullptr)
	{
	}
	
	void attach(attachable_type* attachable)
	{
		Lock lock(*this);

		if (mAttached == attachable)
			return;

		if (mAttached)
		{
			mAttached->_detach(this);
		}
		mAttached = attachable;
		if (mAttached)
		{
			mAttached->_attach(this);
		}
		on_attached();
	}

	attachable_type* attached()
	{
		return mAttached;
	}

	const attachable_type* attached() const
	{
		return mAttached;
	}
private:
	virtual void on_attached() = 0;

	void _detach_silent()
	{
		mAttached = nullptr;
	}

private:
	attachable_type* mAttached;
};



}

#endif // _TILEY_PANEL_LAYER_HPP