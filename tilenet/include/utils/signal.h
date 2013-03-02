#pragma once
#ifndef _SIGNAL_H
#define _SIGNAL_H



#include "settings.h"

namespace utils {


template<typename T>
class Signal
{
public:
	Signal(const boost::function<T>& func)
		:	mFunction(func),
			mMarked(false)
	{
	}
	
	const boost::function<T>& signal()
	{
		mMarked = true;
		return mFunction;
	}

	void satisfiy()
	{
		mMarked = false;
	}

	bool isMarked()
	{
		return mMarked;
	}
private:
	bool						mMarked;
	const boost::function<T>	mFunction;
};







}



#endif