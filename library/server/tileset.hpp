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

	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();
	virtual OVERRIDE void flush();
};



}


#endif