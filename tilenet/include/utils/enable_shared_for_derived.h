#pragma once
#ifndef _SHAREDDERIVE_H
#define _SHAREDDERIVE_H

#include "settings.h"

namespace utils {


template<typename Child, class TopMost>
class enable_shared_for_derived
{
public:

	shared_ptr<Child> shared_from_derived_this()
	{
		Child* c = static_cast<Child*>(this);
		return boost::static_pointer_cast<Child>(c->shared_from_this());
	}

	shared_ptr<Child const> shared_from_derived_this() const
	{
		Child const* c = static_cast<Child const*>(this);
		return boost::static_pointer_cast<Child const>(c->shared_from_this());
	}
};




}



#endif