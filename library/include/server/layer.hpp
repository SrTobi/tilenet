#pragma once
#ifndef _LAYER_HPP
#define _LAYER_HPP


#include "settings.hpp"
#include "utils/link_graph.hpp"
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

	virtual override void destroy() = 0;
	virtual override shared_ptr<TilenetObject> clone() = 0;
	virtual override void flush() = 0;
};



}


#endif