#pragma once
#ifndef _PTR_HPP
#define _PTR_HPP


#include "settings.hpp"

class TilenetWeakObject;

template<typename Type>
class weakptr;

template<typename Type>
class ptr
{
public:
	typedef Type			ptr_type;
	typedef ptr<Type>		this_type;
	typedef weakptr<Type>	weak_type;

	ptr()
		: mPtr(nullptr)
	{
	}

	ptr(ptr_type* p)
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
		reset(weak->unweak().get());
	}

	~ptr()
	{
		reset(nullptr);
	}

	this_type operator =(ptr_type* p)
	{
		reset(p);
		return *this;
	}

	this_type operator =(const this_type& other)
	{
		reset(other.mPtr);
		return *this;
	}

	this_type operator =(const weak_type& other)
	{
		reset(weak->unweak().get());
		return *this;
	}

	void reset(ptr_type* p)
	{
		if(mPtr)
		{
			if(mPtr->subref() == 0)
			{
				delete mPtr;
			}
		}

		mPtr = p;

		if(mPtr)
		{
			mPtr->addref();
		}
	}

	ptr_type* get() const
	{
		return mPtr;
	}

	ptr_type* operator ->() const
	{
		return mPtr;
	}

	ptr_type& operator *() const
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
	ptr_type* mPtr;
};


template<typename Type>
class weakptr
{
public:
	typedef Type				ptr_type;
	typedef TilenetWeakObject	weak_type;
	typedef weakptr<Type>		this_type;
	typedef ptr<Type>			refptr_type;

	weakptr()
		: mPtr(nullptr)
	{
	}

	weakptr(const refptr_type& rp)
		: mPtr(nullptr)
	{
		reset(rp->weak());
	}

	weakptr(const this_type& other)
		: mPtr(nullptr)
	{
		reset(other.mPtr);
	}

	~weakptr()
	{
		if(mPtr)
		{
			mPtr->subref();
		}
	}

	this_type& operator =(const refptr_type& rp)
	{
		reset(rp->weak());
		return *this;
	}

	this_type& operator =(const this_type& other)
	{
		reset(other.mPtr);
		return *this;
	}

private:
	void reset(weak_type* p)
	{
		if(mPtr)
		{
			mPtr->subref();
		}

		mPtr = p;

		if(mPtr)
		{
			mPtr->addref();
		}
	}
private:
	weak_type* mPtr;
};


#endif