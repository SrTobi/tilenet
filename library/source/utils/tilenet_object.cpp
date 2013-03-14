#include "includes.hpp"

/// @cond DEV

#include <boost/smart_ptr/detail/spinlock_pool.hpp>
#include "utils/tilenet_object.hpp"

using boost::detail::spinlock;

static const int SpinLockBaseId = 92137;
typedef boost::detail::spinlock_pool<SpinLockBaseId>	spinlock_pool_for_count;
typedef boost::detail::spinlock_pool<SpinLockBaseId+1>	spinlock_pool_for_obj;

// ******************************* tilenet object ******************************* //
/**
 * @brief Inits the recount to 0
 *
 **/
TilenetObject::TilenetObject()
	: mCount(0)
{

}


TilenetObject::~TilenetObject()
{
	// mWeak must be null because it should have been reseted by subref!
	tnAssert(!mWeak || !mWeak->mObj);
}

/**
 * @brief Returns the reference counting
 *
 * \return The number of references pointing to this class
 **/
size_t TilenetObject::refCount() const
{
	return mCount;
}

/**
 * @brief Returns the weak counting 
 *
 * \return The number of weak references pointing to this class
 **/
size_t TilenetObject::weakCount() const
{
	// subtract the one pointer this class is holding
	return mWeak? (mWeak->mCount - 1) : 0;
}

/**
 * @brief Returns a weak pointer to this object
 *
 * \return A weak pointer to this object
 *
 **/
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

/**
 * @brief Increases the reference counter
 *
 **/
void TilenetObject::addref() const
{
	spinlock_pool_for_count::scoped_lock lock(&mCount);
	++mCount;
}

/**
 * @brief Decreases the reference counter
 *
 * If the reference counter is set to 0 by this function the
 * weak reference is uncoupled.
 * 
 * \return if the object should continue to live
 *
 * \note if false is returned, this object should be deleted immediately
 **/
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

/**
 * @brief Decreases the ref count of an object an deletes it if necessary
 *
 * \param obj The object which should be released.
 **/
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





/// @endcond
