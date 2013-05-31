#include "includes.hpp"
#include "std_tileset.hpp"



namespace srv {




StdTileset::StdTileset()
	: mNextId(1)
{
}

StdTileset::~StdTileset()
{
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

StdTileset& StdTileset::Inst()
{
	static StdTileset inst;
	return inst;
}






}