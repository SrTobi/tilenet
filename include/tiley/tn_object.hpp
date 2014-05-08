#pragma once
#ifndef _TILEY_TNOBJECT_HPP
#define _TILEY_TNOBJECT_HPP


#include "tilenet.h"
#include "config.hpp"

namespace tiley {

typedef TNOBJ ObjectHandle;

class TNObject
{
public:
	inline TNObject()
		: mObject(nullptr)
	{
	}

	inline TNObject(ObjectHandle obj)
		: mObject(obj)
	{
	}
	
	inline virtual ~TNObject()
	{
		destroy();
	}
	
	inline ObjectHandle native_ref() const { return mObject; }
	
protected:
	inline void reset(ObjectHandle obj)
	{
		if (obj != native_ref())
		{
			destroy();
			mObject = obj;
		}
	}
	
private:
	inline void destroy()
	{
		if(mObject)
			Impl::DestroyObject(mObject);
		mObject = nullptr;
	}
	
private:
	ObjectHandle mObject;
};

class TNObjectHandle: public TNObject
{
public:
	inline TNObjectHandle()
	{
	}

	inline TNObjectHandle(ObjectHandle obj)
		: TNObject(obj)
	{
	}

	inline virtual ~TNObjectHandle()
	{
	}

	inline void reset(ObjectHandle obj)
	{
		TNObject::reset(obj);
	}
};

}



#endif