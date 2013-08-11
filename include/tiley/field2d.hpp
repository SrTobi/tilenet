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

private:
	typename storage_type::size_type calcPosition(const size_type&  pos)
	{
		return TNPOS_INDEX(pos.x, pos.y, mSize.w);
	}

	virtual void blit( const Point& pos, const value_type& e, const Rect& size);
	virtual void blit( const Point& pos, const FieldSection<value_type>& field );

private:
	storage_type mStorage;
	size_type mSize;
};


template<typename Elem>
class FieldSection
	: public Blitable<Elem>
{
	template<typename E>
	friend class Field2D;
public:
	typedef std::vector<Elem> storage_type;
	typedef Rect size_type;
	typedef typename storage_type::value_type value_type;
	typedef typename storage_type::reference reference;
	typedef typename storage_type::const_reference const_reference;
	typedef typename storage_type::pointer pointer;
	typedef typename storage_type::const_pointer const_pointer;
	typedef Field2D<Elem> ref_type;
public:
	FieldSection(ref_type& ref, const Point& p, const size_type& size)
		: mFieldRef(ref)
		, mPosition(p)
		, mSize(size)
	{
		// TODO: check values
	}

	FieldSection(FieldSection& other, const Point& p, const size_type& size)
		: mFieldRef(other.mFieldRef)
		, mPosition(other.mPosition + p)
		, mSize(size)
	{
		// TODO: check values
	}

	~FieldSection()
	{
	}


	reference at(const size_type& rpos)
	{
		auto pos = rpos + mPosition;
		return mStorage[calcPosition(pos)];
	}

	const_reference at(const size_type& rpos) const
	{
		auto pos = rpos + mPosition;
		return mStorage[calcPosition(pos)];
	}

	const size_type& size() const
	{
		return mSize;
	}

private:
	virtual void blit( const Point& pos, const value_type& v, const Rect& size)
	{
		if(p.x >= mSize.w || p.y >= mSize.h)
			return;

		Point p(std::max(pos.x, 0), std::max(pos.y, 0));
		size_type nsize(std::min(mSize.w, size.w - (p.x - pos.x)), std::min(mSize.h, size.h - (p.y - pos.y)));
		mFieldRef->blit(pos + mPosition, v, nsize);
	}

	virtual void blit( const Point& pos, const FieldSection<value_type>& field )
	{
		if(p.x >= mSize.w || p.y >= mSize.h)
			return;

		Point p(std::max(pos.x, 0), std::max(pos.y, 0));
		size_type size(std::min(mSize.w, field.mSize.w - (p.x - pos.x)), std::min(mSize.h, field.mSize.h - (p.y - pos.y)));
		mFieldRef->blit(p + mPosition, FieldSection<value_type>(*this, p - pos, size));
	}


private:
	ref_type& mFieldRef;
	Point mPosition;
	size_type mSize;
};


template<typename Elem>
void Field2D<Elem>::blit( const Point& pos, const value_type& e, const Rect& size)
{
	Point top_left(std::max(0, pos.x), std::max(0, pos.y));
	auto width = std::min(mSize.w - top_left.x, size.w);
	auto height = std:min(mSize.h - top_left.y, size.h);

	storage_type::iterator destIt = newStorage.begin() + calcPosition(top_left);

	for(int i = 0; i < height; ++i)
	{
		std::fill(srcIt, srcIt + width, e);

		std::advance(destIt, size().w);
	}	
}

template<typename Elem>
void Field2D<Elem>::blit( const Point& pos, const FieldSection<value_type>& field )
{
	auto& otherStoreage = field.mFieldRef.storage();

	Point top_left(std::max(0, pos.x), std::max(0, pos.y));
	auto width = std::min(mSize.w - top_left.x, field.mSize.w);
	auto height = std:min(mSize.h - top_left.y, field.mSize.h);

	storage_type::iterator srcIt = otherStoreage.begin() + field.mFieldRef.calcPosition(field.mPosition);
	storage_type::iterator destIt = newStorage.begin() + calcPosition(top_left);

	for(int i = 0; i < height; ++i)
	{
		std::copy(srcIt, srcIt + width, destIt);

		std::advance(srcIt, field.mFieldRef.size().w);
		std::advance(destIt, size().w);
	}	
}


}


#endif
