#pragma once
#ifndef _TILEY_TILE_LAYER_HPP
#define _TILEY_TILE_LAYER_HPP

#include <cstring>

#include "field2d.hpp"
#include "layer.hpp"
#include "blitable.hpp"

namespace tiley {

template<typename Ch = TILEY_DEFAULT_CHAR, typename Mutex = TILEY_DEFAULT_MUTEX>
class TileLayer
	: public Layer
	, public Blitable<TNTILE>
{
public:
	typedef std::basic_string<Ch>					string;
	typedef typename Blitable<TNTILE>::size_type	size_type;
	typedef Ratio									ratio_type;
	typedef Layer									base_type;
public:
	TileLayer(const size_type& size, const Ratio& ratio = Ratio(1.0f, 1.0f), TNFLAG flags = 0) 
		: mContent(size)
		, mUpdated(size)
	{
		init();
		reset(Impl::CreateTilelayer(size, ratio, std::wstring(L"std"), flags));
	}

	TileLayer(const size_type& size, const string& aspect, const Ratio& ratio = Ratio(1.0f, 1.0f), TNFLAG flags = 0) 
		: mContent(size)
		, mUpdated(size)
	{
		init();
		reset(Impl::CreateTilelayer(size, ratio, aspect, flags));
	}


	inline virtual const size_type& size() const
	{
		return mContent.size();
	}

private:
	void init()
	{
		TNTILE nullTile;
		memset(&nullTile, 0, sizeof(nullTile));
		mContent.clear(nullTile);
		mUpdated.clear(0);
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
		if(pos.isIn(size()))
			return mContent.at(pos);
		else
			return this->defaultValue();
	}

	virtual void set(const Point& pos, const value_type& e)
	{
		if(pos.isIn(size()))
		{
			mContent.at(pos) = e;
			mUpdated.at(pos) = 1;
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
	mutable Field2D<TNBOOL> mUpdated;

	mutable Mutex mMutex;
};





}



#endif
