#pragma once
#ifndef _FIELD_HPP
#define _FIELD_HPP


#include "settings.hpp"
#include "vector.hpp"


template<typename Type>
class Field
{
public:
	typedef std::vector<Type> storage_type;
	typedef typename storage_type::value_type value_type;
	typedef Rect size_type;
	typedef typename storage_type::reference reference;
	typedef typename storage_type::const_reference const_reference;
	typedef typename storage_type::pointer pointer;
	typedef typename storage_type::const_pointer const_pointer;

	Field(const size_type& size)
		: mStorage(size.area())
		, mSize(size)
	{
	}


	Field(const Field& other)
		: mStorage(other.mStorage)
		, mSize(other.mSize)
	{
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

private:
	typename storage_type::size_type calcPosition(const size_type&  pos)
	{
		return mSize.w * pos.y + pos.x;
	}

private:
	storage_type mStorage;
	size_type mSize;
};








#endif