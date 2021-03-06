#pragma once
#ifndef _TILEY_STENCIL_HPP
#define _TILEY_STENCIL_HPP

#include "field2d.hpp"
#include "mask.hpp"


namespace tiley {


template<typename Elem, typename Mutex = TILEY_DEFAULT_MUTEX>
class Stencil
	: public Blitable<Elem>
	, public AutoObject
{
public:	
	typedef Mutex mutex_type;
	typedef Elem value_type;
	typedef Blitable<Elem> target_type;
	typedef typename target_type::size_type size_type;
public:

	Stencil(target_type& target)
		: mTarget(target)
		, mPos(0, 0)
		, mSize(target->size())
	{
	}

	Stencil(target_type& target, const size_type& pos, const size_type& size)
		: mTarget(target)
		, mPos(pos)
		, mSize(size)
	{
		assert(size.w >= 0 && size.h >= 0);
	}

	~Stencil()
	{
		free(mMask);
	}

	virtual const size_type& size() const
	{
		return mSize;
	}

	bool masked(const size_type& pos)
	{
		return !mMask || mMask->masked(pos);
	}

protected:


	virtual const value_type& get( const Point& pos ) const
	{
		if(pos.isIn(mSize) && masked(pos))
			return mTarget.get(mPos + pos);
		else
			return this->defaultValue();
	}

	virtual void set(const Point& pos, const value_type& e)
	{
		if(pos.isIn(mSize) && masked(pos))
			mTarget.set(e, mPos + pos);
		else
		{
			assert(this->defaultValue());
		}
	}

	virtual void lock() const
	{
		mTarget.lock();
	}

	virtual void unlock() const
	{
		mTarget.unlock();
	}

	virtual void do_flush() const
	{
		mTarget.flush();
	}

	virtual void apply_to_children( void (* ) (Flushable&)) const
	{
		// no children
	}

private:
	target_type& mTarget;
	Mask<mutex_type>* mMask;
	size_type mSize;
	size_type mPos;
};




}


#endif
