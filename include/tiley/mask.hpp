#pragma once
#ifndef _TILEY_MASK_HPP
#define _TILEY_MASK_HPP

#include "field2d.hpp"
#include "auto_object.hpp"
#include "blitable.hpp"

namespace tiley {

template<typename Mutex = TILEY_DEFAULT_MUTEX>
class Mask
	: public Blitable<bool>
	, public AutoObject
{
public:
	typedef bool value_type;
	typedef typename Blitable<bool>::size_type size_type;
public:
	Mask(const size_type& size)
		: mMask(size)
	{
		clear();
	}

	bool mask(const size_type& pos, bool newVal) const
	{
		return mMask.at(pos) = newVal;
	}

	bool masked(const size_type& pos) const
	{
		return mMask.at(pos);
	}

	void clear(bool v = true)
	{
		mMask.clear(v);
	}

	virtual const size_type& size() const
	{
		return mMask.size();
	}

private:

	virtual const value_type& get( const Point& pos ) const
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual void set( const value_type& e, const Point& pos )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	virtual void lock() const
	{
		mMutex.lock();
	}

	virtual void unlock() const
	{
		mMutex.unlock();
	}

	virtual void do_flush() const
	{
		// no flush
	}

	virtual void apply_to_children( void (* ) (Flushable&)) const
	{
		// No children
	}

private:
	Field2D<bool> mMask;
	mutable Mutex mMutex;
};



}


#endif