#include "includes.hpp"

/// @cond DEV
#include "utils/tilenet_object.hpp"

// ******************************* tilenet object ******************************* //

TilenetObject::TilenetObject()
	: mSelf(this)
{
}

TilenetObject::~TilenetObject()
{
}

void TilenetObject::Destroy( TilenetObject* obj )
{
	tnAssert(obj);
	obj->destroy();
	obj->mSelf.reset();
}




/// @endcond
