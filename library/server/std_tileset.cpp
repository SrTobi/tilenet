#include "includes.hpp"
#include "std_tileset.hpp"



namespace srv {




StdTileset::StdTileset()
	: mTileIdToNameMapping(1)
{
}

StdTileset::~StdTileset()
{
}



TNID StdTileset::getTileId( const string& name )
{
	std::lock_guard<std::mutex> lock(mMutex);

	// Find next free id
	auto it = mTileNameToIdMapping.find(name);
	if(it != mTileNameToIdMapping.end())
	{
		return it->second;
	}

	TNID newId = mTileIdToNameMapping.size();

	mTileIdToNameMapping.push_back(name);
	mTileNameToIdMapping[name] = newId;

	return newId;
}


const string& StdTileset::getTileName( TNID id ) const
{
	std::lock_guard<std::mutex> lock(mMutex);
	
	if(id >= mTileIdToNameMapping.size() || mTileIdToNameMapping.at(id).empty())
	{
		NOT_IMPLEMENTED();
	}

	return mTileIdToNameMapping.at(id);
}

StdTileset& StdTileset::Inst()
{
	static StdTileset inst;
	return inst;
}






}