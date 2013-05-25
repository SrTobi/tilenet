#pragma once
#ifndef _TILESET_HPP
#define _TILESET_HPP

#include "settings.hpp"
#include "network/v1.0/protocol_tile.hpp"


namespace srv {

class Tileset
	: public TilenetObject
	, public IdObject<Tileset>
{
public:
	Tileset(const string& name, TNFLAG flags);
	~Tileset();


	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();
	virtual OVERRIDE void flush();

	virtual net::PTile&& constructTile(TNTILE* tile) = 0;
	const string& name() const;

private:
	string mName;
	TNFLAG mFlags;
};



}


#endif