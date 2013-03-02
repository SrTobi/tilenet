#pragma once
#ifndef _IDVECTOR_H
#define _IDVECTOR_H

#include "settings.h"


namespace utils {


template<typename T>
class IdVector
{
public:
	
	int requestId()
	{
		if(mFreeSlots.size())
		{
			int freeslot = mFreeSlots.front();
			mFreeSlots.pop();
			return freeslot;
		}

		mVector.push_back(T());
		return mVector.size() - 1;
	}

	void remove(int id)
	{
		mVector[id] = T();
		mFreeSlots.push(id);
	}

	const T& operator [](int id) const
	{
		return mVector[id];
	}
	
	T& operator [](int id)
	{
		return mVector[id];
	}

private:
	std::vector<T>	mVector;
	std::queue<int>	mFreeSlots;
};






}




#endif