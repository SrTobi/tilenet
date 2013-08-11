#pragma once
#ifndef _TILEY_TILE_LAYER_HPP
#define _TILEY_TILE_LAYER_HPP


#include "field2d.hpp"
#include "layer.hpp"
#include "blitable.hpp"

namespace tiley {

template<typename Ch, typename Mutex>
class TileLayer
	: public Layer<Ch, Mutex>
	, public Blitable<TNTILE>
{
public:
	typedef Rect size_type;
	typedef Layer<Ch, Mutex> base_type; 
public:
	TileLayer(const size_type& size)
	{
	}

private:
	virtual void do_flush() const
	{
		Impl::UpdateTilelayer(native_ref(), mContent, mUpdated);
		mUpdated.clear(0);
	}

	virtual void apply_to_children(void (* ) (Flushable&)) const
	{
		// no children
	}

	virtual const value_type& get( const Point& pos ) const
	{
		if(pos.isIn(mSize))
			return mContent.at(mPos + pos);
		else
			return defaultValue();
	}

	virtual void set(const value_type& e, const Point& pos )
	{
		if(pos.isIn(mSize) && masked(pos))
		{
			mContent.at(mPos + pos) = e;
			mUpdated.at(mPos + pos) = 1;
		}else
		{
			defaultValue();
		}
	}

	virtual void lock() const
	{
		mMutex.lock();
	}

	virtual void unlock() const
	{
		mMutex.unlock();
	}


private:
	Field2D<TNTILE> mContent;
	Field2D<TNBOOL> mUpdated;
	mutable Mutex mMutex;
};





}



#endif