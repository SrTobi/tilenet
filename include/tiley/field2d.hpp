#pragma once
#ifndef _TILEY_FIELD2D_HPP
#define _TILEY_FIELD2D_HPP


#include "blitable.hpp"
#include "vector.hpp"

namespace tiley {


template<typename Elem>
class Field2D
{
public:
	typedef std::vector<Elem> storage_type;
	typedef Rect size_type;
	typedef typename storage_type::value_type value_type;
	typedef typename storage_type::reference reference;
	typedef typename storage_type::const_reference const_reference;
	typedef typename storage_type::pointer pointer;
	typedef typename storage_type::const_pointer const_pointer;

public:
	Field2D(const size_type& size)
		: mStorage(size.area())
		, mSize(size)
	{
	}


	Field2D(const Field2D& other)
		: mStorage(other.mStorage)
		, mSize(other.mSize)
	{
	}

	void resize(const size_type& size)
	{
		storage_type newStorage(size.area());

		storage_type::iterator srcIt = mStorage.begin();
		storage_type::iterator destIt = newStorage.begin();

		auto w = std::min(mSize.w, size.w);

		while(srcIt != mStorage.end() && destIt != newStorage.end())
		{
			std::copy(srcIt, srcIt + w, destIt);

			std::advance(srcIt, mSize.w);
			std::advance(destIt, size.w);
		}	

		mStorage = std::move(newStorage);
		mSize = size;
	}

	reference at(const size_type& pos)
	{
		return mStorage[calcPosition(pos)];
	}

	const_reference at(const size_type& pos) const
	{
		return mStorage[calcPosition(pos)];
	}

	storage_type& storage()
	{
		return mStorage;
	}

	const storage_type& storage() const
	{
		return mStorage;
	}

	const size_type& size() const
	{
		return mSize;
	}

	void clear(const value_type& v)
	{
		std::fill(mStorage.begin(), mStorage.end(), v);
	}

private:
	typename storage_type::size_type calcPosition(const size_type&  pos)
	{
		return TNPOS_INDEX(pos.x, pos.y, mSize.w);
	}


private:
	storage_type mStorage;
	size_type mSize;
};

}


#endif
