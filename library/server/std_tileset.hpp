#pragma once
#ifndef _STD_TILESET_HPP
#define _STD_TILESET_HPP



#include <mutex>
#include <unordered_map>
#include "settings.hpp"
#include "tileset.hpp"


namespace srv {

class StdTileset
	: public Tileset
{
public:
	StdTileset(const string& name, TNFLAG flags);
	~StdTileset();

	TNID registerTile(const string& name);
	TNID registerTile(const string& name, TNID id);

	virtual OVERRIDE void destroy();
	virtual OVERRIDE shared_ptr<TilenetObject> clone();
	virtual OVERRIDE void flush();

	virtual OVERRIDE net::PTile constructTile(TNTILE* tile);
	const string& getTileName(TNID id) const;

private:
	mutable std::mutex mMutex;
	std::unordered_map<TNID, string> mTiles;
	TNID mNextId;
};







}

#endif