#pragma once
#ifndef _SYNCRONIZED_H
#define _SYNCRONIZED_H

#include "settings.h"

namespace utils {



template<bool cond, typename type>
struct make_const_if
{
	typedef type result;
};

template<typename type>
struct make_const_if<true, type>
{
	typedef const type result;
};


template<typename T, typename MUTEX_TYPE>
class Syncronized;

template<typename T, typename GUARD_TYPE = boost::lock_guard<typename T:: _MutexType> >
class syncronize
{
public:
	syncronize(T& sobj)
		:	mGuard(sobj.mMutex),
			mObject(&(sobj.mObject))
	{
	}

	typename make_const_if<boost::is_const<T>::value, typename T::_ValueType>::result& ref()
	{
		return *mObject;
	}

	typename make_const_if<boost::is_const<T>::value, typename T::_ValueType>::result* operator ->()
	{
		return mObject;
	}

private:
	GUARD_TYPE				mGuard;
	typename make_const_if<boost::is_const<T>::value, typename T::_ValueType>::result*	mObject;
};

template<typename T, typename MUTEX_TYPE, typename GUARD_TYPE>
class syncronize<Syncronized<T*, MUTEX_TYPE>, GUARD_TYPE>
{
	typedef T*	TP;
public:
	
	syncronize(Syncronized<T*, typename Syncronized<T*, MUTEX_TYPE>::_MutexType>& sobj)
		:	mGuard(sobj.mMutex),
			mObject(&(sobj.mObject))
	{
	}

	TP& ref()
	{
		return *mObject;
	}

	TP operator ->()
	{
		return *mObject;
	}

private:
	GUARD_TYPE	mGuard;
	TP*	mObject;
};


template<typename T, typename MUTEX_TYPE = boost::mutex>
class Syncronized
{
	template<typename _T, typename _GUARD_TYPE>
	friend class syncronize;
public:
	typedef T			_ValueType;
	typedef MUTEX_TYPE	_MutexType;
public:
	Syncronized()
	{
	}
	
	Syncronized(const T& _obj)
		: mObject(_obj)
	{
	}

private:
	mutable MUTEX_TYPE mMutex;
	T mObject;
};
/*
template<typename T, typename MUTEX_TYPE>
class Syncronized<T*, MUTEX_TYPE>
	: public MUTEX_TYPE
{
public:
	Syncronized(T* p = nullptr)
		: mPointer(p)
	{
	}

	Syncronized(const Syncronized& other)
	{
		mPointer = other.mPointer;
	}
	
	Syncronized& operator =(const T* p)
	{
		mPointer = p;
		return *this;
	}

	Syncronized& operator =(const Syncronized& other)
	{
		mPointer = other.mPointer;
		return *this;
	}

	T* operator ->() 
	{
		return mPointer;
	}

private:
	T*	mPointer;
};


*/

}






#endif
