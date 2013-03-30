#pragma once
#ifndef _TILESET_HPP
#define _TILESET_HPP

#include "settings.hpp"


namespace srv {

class Tileset
	: public TilenetObject
	, public IdObject<Tileset>
{
public:
	Tileset(const string& name, TNFLAG flags);
	~Tileset();

	TNID registerTile(const string& name);
	TNID registerTile(const string& name, TNID id);

	virtual override void destroy();
	virtual override shared_ptr<TilenetObject> clone();
	virtual override void flush();
};



}


#endif