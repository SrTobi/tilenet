#pragma once
#ifndef _TILEY_AUTO_OBJECT_HPP
#define _TILEY_AUTO_OBJECT_HPP




namespace tiley {

class AutoObject
{
public:
	inline AutoObject()
		: mAutoFree(true)
		, mAlive(true)
	{

	}

	virtual ~AutoObject()
	{
	}

	inline bool autoFree() const
	{
		return mAutoFree;
	}

	inline bool autoFree(bool aF)
	{
		return (mAutoFree = aF);
	}

protected:
	inline void free(AutoObject* obj)
	{
		mAlive = false;
		if(obj && obj->autoFree() && obj->alive())
		{
			delete obj;
		}
	}

	template<typename Iter>
	inline void free(Iter begin, Iter end)
	{
		mAlive = false;
		while(begin != end)
		{
			free(*begin);
			++begin;
		}
	}

	inline bool alive() const { return mAlive; }

private:
	bool mAutoFree;
	bool mAlive;
};



}



#endif