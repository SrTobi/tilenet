#pragma once
#ifndef _TILEY_LOCKABLE_HPP
#define _TILEY_LOCKABLE_HPP


namespace tiley {

class Lock;

class Lockable
{
	friend class Lock;
protected:
	virtual ~Lockable() {};
	virtual void lock() const = 0;
	virtual void unlock() const = 0;
};


class Lock
{
public:
	inline Lock(const Lockable& la)
		: mLockable(la)
	{
		mLockable.lock();
	}
	
	inline ~Lock()
	{
		mLockable.unlock();
	}
	
private:
	const Lockable& mLockable;
};

}

#endif