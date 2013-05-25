#include "includes.hpp"
#include "std_tileset.hpp"



namespace srv {




StdTileset::StdTileset( const string& name, TNFLAG flags )
	: Tileset(name, flags)
	, mNextId(1)
{
}

StdTileset::~StdTileset()
{
}

OVERRIDE void StdTileset::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> StdTileset::clone()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void StdTileset::flush()
{
	NOT_IMPLEMENTED();
}


TNID StdTileset::registerTile( const string& name )
{
	std::lock_guard<std::mutex> lock(mMutex);

	// Find next free id
	while(mTiles.find(mNextId) != mTiles.end())
	{
		++mNextId;
	}

	mTiles[mNextId] = name;

	return mNextId++;
}

TNID StdTileset::registerTile( const string& name, TNID id )
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mTiles.find(id) != mTiles.end())
	{
		NOT_IMPLEMENTED();
	}

	mTiles[id] = name;

	return id;
}

OVERRIDE net::PTile StdTileset::constructTile(TNTILE* tile)
{
	tnAssert(tile->tileset == id());

	auto& data = tile->data.stdset;

	net::PTile ptile(id(), data.id, data.modifier, data.color);
	return std::move(ptile);
}

const string& StdTileset::getTileName( TNID id ) const
{
	std::lock_guard<std::mutex> lock(mMutex);
	auto it = mTiles.find(id);

	if(it == mTiles.end())
	{
		NOT_IMPLEMENTED();
	}

	return it->second;
}



}