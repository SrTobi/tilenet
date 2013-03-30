#pragma once
#ifndef _LAYER_HPP
#define _LAYER_HPP


#include "settings.hpp"
#include "utils/link_graph.hpp"

namespace srv {

class Frame;

class Layer
	: public TilenetObject
{
public:
	Layer();
	~Layer();

	virtual override void destroy();
	virtual override shared_ptr<TilenetObject> clone();
};



}


#endif