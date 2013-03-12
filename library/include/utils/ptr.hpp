#pragma once
#ifndef _PTR_HPP
#define _PTR_HPP

#include <type_traits>

#include "settings.hpp"

class TilenetWeakObject;

template<typename Type>
class weakptr;

template<typename Type>
class ptr
{
public:
	typedef Type			value_type;
	typedef ptr<Type>		this_type;
	typedef weakptr<Type>	weak_type;

	ptr()
		: mPtr(nullptr)
	{
	}

	ptr(const std::nullptr_t&)
		: mPtr(nullptr)
	{
	}

	ptr(value_type* p)
		: mPtr(nullptr)
	{
		reset(p);
	}

	ptr(const this_type& other)
		: mPtr(nullptr)
	{
		reset(other.mPtr);
	}

	ptr(const weak_type& weak)
		: mPtr(nullptr)
	{
		reset(weak.unweak().get());
	}

	template<typename Other>
	ptr(const ptr<Other>& other)
	{
		static_assert(std::is_base_of<value_type, Other>::value, "Can not convert from 'Other' to 'value_type'!");
		reset(static_cast<value_type*>(other.get()));
	}
	
	~ptr()
	{
		reset(nullptr);
	}

	this_type operator =(value_type* p)
	{
		reset(p);
		return *this;
	}

	this_type& operator =(const std::nullptr_t&)
	{
		reset(nullptr);
		return *this;
	}

	this_type& operator =(const this_type& other)
	{
		reset(other.mPtr);
		return *this;
	}

	this_type& operator =(const weak_type& other)
	{
		reset(weak->unweak().get());
		return *this;
	}


	void reset(value_type* p)
	{
		if(mPtr)
		{
			TilenetObject::Release(mPtr);
		}

		mPtr = p;

		if(mPtr)
		{
			mPtr->addref();
		}
	}

	value_type* get() const
	{
		return mPtr;
	}

	value_type* operator ->() const
	{
		return mPtr;
	}

	value_type& operator *() const
	{
		tnAssert(mPtr);
		return *mPtr;
	}

	bool operator !() const
	{
		return mPtr == nullptr;
	}

	operator bool() const
	{
		return mPtr != nullptr;
	}

private:
	value_type* mPtr;
};


template<typename Type>
class weakptr
{
public:
	typedef Type				value_type;
	typedef TilenetWeakObject	weak_type;
	typedef weakptr<value_type>	this_type;

	weakptr()
	{
	}

	weakptr(const std::nullptr_t&)
	{
	}

	weakptr(const ptr<value_type>& p)
	{
		reset(p);
	}

	weakptr(const this_type& other)
	{
		reset(other);
	}

	~weakptr()
	{
	}

	this_type& operator =(const std::nullptr_t&)
	{
		reset(nullptr);
		return *this;
	}

	this_type& operator =(const ptr<value_type>& p)
	{
		reset(p);
		return *this;
	}

	this_type& operator =(const this_type& other)
	{
		reset(other.mWeakPtr);
		return *this;
	}

	void reset(const std::nullptr_t&)
	{
		mWeakPtr = nullptr;
	}

	void reset(const ptr<value_type>& obj)
	{
		if(obj)
			mWeakPtr = obj->weak();
		else
			mWeakPtr = nullptr;
	}

	void reset(const this_type& other)
	{
		mWeakPtr = other.mWeakPtr;
	}

	ptr<value_type> unweak()
	{
		if(!mWeakPtr)
			return nullptr;

		ptr<TilenetObject> objPtr = mWeakPtr->unweak();

		if(!objPtr)
		{
			mWeakPtr = nullptr;
			return nullptr;
		}

		value_type* converted = tilenet_vcast<value_type*>(objPtr.get());
		tnAssert(converted);
		return converted;
	}

private:
	ptr<weak_type> mWeakPtr;
};


#endif