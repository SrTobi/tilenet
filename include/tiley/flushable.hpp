#pragma once
#ifndef _TILEY_FLUSHABLE_HPP
#define _TILEY_FLUSHABLE_HPP

#include <atomic>
#include <cassert>

namespace tiley {

class Flushable
{
public:
	Flushable()
	{
	
	}
	
	virtual ~Flushable() {}
	
	inline void flush()
	{
		if(!frozen())
			do_flush();
	}
	
	inline void flushAll()
	{
		ApplyFlushAll(*this);
	}
	
	virtual void freeze()
	{
		ApplyFreeze(*this);
	}
	
	virtual void unfreeze()
	{
		ApplyUnFreeze(*this);
	}
	
	bool frozen() const
	{
		return mFreezeCount > 0;
	}
	
protected:
	virtual void do_flush() const = 0;
	virtual void apply_to_children(void (*)(Flushable&)) const = 0;
	
	static void ApplyFlushAll(Flushable& f)
	{
		if(!f.frozen())
		{
			f.do_flush();
			f.apply_to_children(&Flushable::ApplyFlushAll);
		}
	}
	
	static void ApplyFreeze(Flushable& f)
	{
		bool frozen = f.frozen();
		
		f.mFreezeCount++;
		
		if(!frozen)
		{
			f.apply_to_children(&Flushable::ApplyFreeze);
		}
	}
	
	static void ApplyUnFreeze(Flushable& f)
	{
		assert(f.frozen());
		
		f.mFreezeCount--;
		
		if(!f.frozen())
		{
			f.apply_to_children(&Flushable::ApplyUnFreeze);
		}
	}
private:
	std::atomic<unsigned int> mFreezeCount;
};


class AutoFlush
{
public:
	AutoFlush(Flushable& toFlush)
		: mToFlush(toFlush)
	{
	}
	
	~AutoFlush()
	{
		mToFlush.flush();
	}

private:
	Flushable& mToFlush;
};

}
#endif