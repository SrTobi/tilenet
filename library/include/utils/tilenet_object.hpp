#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP

/// @cond DEV

#include <boost/noncopyable.hpp>
#include "settings.hpp"

#include "ptr.hpp"


/**
 * @brief Super class of most of tilenet's objects
 *
 * If an object is a subclass of Tilenetobject it can be used outside of tilenet with the c api.
 *
 **/
struct TilenetObject
{
	template<typename Type, typename TnObject>
	friend class ptr;
public:
	TilenetObject();
	virtual ~TilenetObject();

	virtual size_t destroy() = 0;
	virtual TilenetObject* clone() = 0;
};







/// @endcond

#endif