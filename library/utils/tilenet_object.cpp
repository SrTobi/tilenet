#include "includes.hpp"

/// @cond DEV
#include "utils/tilenet_object.hpp"

// ******************************* tilenet object ******************************* //

TilenetObject::TilenetObject()
{
}

TilenetObject::~TilenetObject()
{
}

void TilenetObject::Destroy( TilenetObject* obj )
{
	tnAssert(obj);
	obj->destroy();

	tnAssert(obj->mSelf);
	obj->mSelf.reset();
}

TNOBJ TilenetObject::initialize()
{
	tnAssert(!mSelf);
	init();
	tnAssert(mSelf);

	return mSelf.get();
}


void TilenetObject::init()
{
	mSelf = shared_from_this();
}





/// @endcond
