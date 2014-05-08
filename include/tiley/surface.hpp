#pragma once
#ifndef _TILEY_SURFACE_HPP
#define _TILEY_SURFACE_HPP

#include "tile.hpp"
#include "blitable.hpp"
#include "flushable.hpp"
#include "attachable.hpp"

namespace tiley {


class Surface
	: public Blitable<RawTile>
	, public Attachable<Surface>
{
public:
	typedef Rect	size_type;
	typedef Blitable<RawTile>						base_type;
public:
	inline Surface(const size_type& size)
		: mContent(size)
		, mUpdated(size)
	{
		init();
	}

	inline virtual const size_type& size() const
	{
		return mContent.size();
	}

	const Field2D<RawTile>& content() const
	{
		return mContent;
	}

	const Field2D<TNBOOL>& updated_content() const
	{
		return mUpdated;
	}

	void clear_update() const
	{
		mUpdated.clear(0);
	}

private:
	inline void init()
	{
		TNTILE nullTile;
		memset(&nullTile, 0, sizeof(nullTile));
		mContent.clear(nullTile);
		mUpdated.clear(0);
	}

protected:
	inline virtual void do_flush() const
	{
		_flush_attached();
	}

	inline virtual void apply_to_children(void (* ) (Flushable&)) const
	{
		// no children
	}

	inline virtual const value_type& get( const Point& pos ) const
	{
		if(pos.isIn(size()))
			return mContent.at(pos);
		else
			return this->defaultValue();
	}

	inline virtual void set(const Point& pos, const value_type& e)
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

	inline virtual void lock() const
	{
	}

	inline virtual void unlock() const
	{
	}

private:
	Field2D<RawTile> mContent;
	mutable Field2D<TNBOOL> mUpdated;
};





}

#endif