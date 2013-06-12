#pragma once
#ifndef _LAYER_HPP
#define _LAYER_HPP


#include "settings.hpp"
#include "network/sendable.hpp"

namespace srv {

class Frame;

class Layer
	: public TilenetObject
	, public IdObject<Layer>
	, public net::Sendable
{
public:
	Layer();
	~Layer();

	virtual OVERRIDE void destroy() = 0;
	virtual OVERRIDE shared_ptr<TilenetObject> clone() = 0;
};



}


#endif
