#include "includes.hpp"

#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include "utils/tilenet_object.hpp"

using boost::detail::spinlock;

static const int SpinLockBaseId = 92137;
typedef boost::detail::spinlock_pool<SpinLockBaseId>	spinlock_pool_for_count;
typedef boost::detail::spinlock_pool<SpinLockBaseId+1>	spinlock_pool_for_obj;

// ******************************* tilenet object ******************************* //
TilenetObject::TilenetObject()
	: mCount(0)
{

}


TilenetObject::~TilenetObject()
{
	// mWeak must be null because it should have been reseted by subref!
	tnAssert(!mWeak || !mWeak->mObj);
}

size_t TilenetObject::refCount() const
{
	return mCount;
}

size_t TilenetObject::weakCount() const
{
	// subtract the one pointer this class is holding
	return mWeak? (mWeak->mCount - 1) : 0;
}


ptr<TilenetWeakObject> TilenetObject::weak()
{
	spinlock_pool_for_obj::scoped_lock lock((TilenetObject*)this);
	if(mWeak)
	{
		return mWeak;
	}

	mWeak = new TilenetWeakObject(this);
	tnAssert((void*)mWeak->mObj == (void*)(TilenetObject*)this);
	return mWeak;
}

void TilenetObject::addref() const
{
	spinlock_pool_for_count::scoped_lock lock(&mCount);
	++mCount;
}

bool TilenetObject::subref() const
{
	spinlock_pool_for_count::scoped_lock lock(&mCount);
	if(mCount > 0)
		--mCount;

	if(mCount == 0)
	{
		spinlock_pool_for_obj::scoped_lock lock2(this);
		if(mWeak)
		{
			mWeak->uncouple();
		}
		return false;
	}
	return true;
}

size_t TilenetObject::destroy()
{
	return 0;
}


void TilenetObject::Release( const TilenetObject* obj )
{
	if(!obj->subref())
	{
		delete obj;
	}
}




// ******************************* tilenet weak object ******************************* //
TilenetWeakObject::TilenetWeakObject(TilenetObject* obj)
	: mObj(obj)
{
	tnAssert(obj);
}

TilenetWeakObject::~TilenetWeakObject()
{

}

override ptr<TilenetWeakObject> TilenetWeakObject::weak()
{
	return this;
}


ptr<TilenetObject> TilenetWeakObject::unweak()
{
	spinlock_pool_for_obj::scoped_lock lock(mObj);
	return mObj;
}

ptr<const TilenetObject> TilenetWeakObject::unweak() const
{
	spinlock_pool_for_obj::scoped_lock lock(mObj);
	return mObj;
}

void TilenetWeakObject::uncouple()
{
	mObj = nullptr;
}

override size_t TilenetWeakObject::destroy()
{
	throw "not implemented!";
}





// ******************************* tilenet id object ******************************* //
TilenetIdObject::TilenetIdObject()
{

}

TilenetIdObject::~TilenetIdObject()
{

}
