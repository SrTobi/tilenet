#include "utils/tilenet_object.hpp"



// ******************************* tilenet object ******************************* //
TilenetObject::TilenetObject()
	: mCount(0)
{

}

TilenetObject::~TilenetObject()
{
	mWeak = nullptr;
}

size_t TilenetObject::refCount() const
{
	return mCount;
}

size_t TilenetObject::weakCount() const
{
	return mWeak? mWeak->mCount : 0;
}


ptr<TilenetWeakObject> TilenetObject::weak()
{
	if(mWeak)
	{
		return mWeak;
	}

	return mWeak = new TilenetWeakObject(this);
}

void TilenetObject::addref() const
{
	++mCount;
}

size_t TilenetObject::subref() const
{

	return mCount == 0 ? 0 : --mCount;
}



// ******************************* tilenet weak object ******************************* //
TilenetWeakObject::TilenetWeakObject( const ptr<TilenetObject>& obj )
	: mObj(obj.get())
{
	tnAssert(obj.get());
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
	return mObj;
}

ptr<const TilenetObject> TilenetWeakObject::unweak() const
{
	return mObj;
}



// ******************************* tilenet id object ******************************* //
TilenetIdObject::TilenetIdObject()
{

}

TilenetIdObject::~TilenetIdObject()
{

}
