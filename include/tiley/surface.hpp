#pragma once
#ifndef _TILEY_SURFACE_HPP
#define _TILEY_SURFACE_HPP

#include <string>
#include "field2d.hpp"
#include "mask.hpp"


namespace tiley {


template<typename Ch, typename Elem>
class Surface
	: public Blitable<Elem>
	, public AutoObject
{
public:
	typedef std::basic_string<Ch> string;
	typedef Elem value_type;
	typedef Blitable<Elem> target_type;
	typedef typename target_type::size_type size_type;
public:

	Surface(target_type& target)
		: mTarget(target)
		, mPos(0, 0)
		, mSize(target->size())
	{
	}

	Surface(target_type& target, const size_type& pos, const size_type& size)
		: mTarget(target)
		, mPos(pos)
		, mSize(size)
	{
		assert(size.w >= 0 && size.h >= 0)
	}

	~Surface()
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
			return defaultValue();
	}

	virtual void set(const value_type& e, const Point& pos )
	{
		if(pos.isIn(mSize) && masked(pos))
			mTarget.set(e, mPos + pos);
		else
		{
			assert(defaultValue());
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
	Mask* mMask;
	size_type mSize;
	size_type mPos;
};




}


#endif