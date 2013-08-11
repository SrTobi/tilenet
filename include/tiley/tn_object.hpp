#pragma once
#ifndef _TILEY_TNOBJECT_HPP
#define _TILEY_TNOBJECT_HPP


#include "tilenet.h"
#include "config.hpp"

namespace tiley {


class TNObject
{
public:
	inline TNObject()
		: mObject(nullptr)
	{
	}

	inline TNObject(TNOBJ obj)
		: mObject(obj)
	{
	}
	
	inline virtual ~TNObject()
	{
		destroy();
	}
	
	inline TNOBJ native_ref() const { return mObject; }
	
protected:
	inline void reset(TNOBJ obj)
	{
		destroy();
		mObject = obj;
	}
	
private:
	inline void destroy()
	{
		if(mObject)
			Impl::DestroyObject(mObject);
	}
	
private:
	TNOBJ mObject;
};



}



#endif