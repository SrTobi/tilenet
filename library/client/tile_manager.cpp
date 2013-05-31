#include "includes.hpp"

#include <fstream>

#include "tile_manager.hpp"
#include "std_tile_pool.hpp"

namespace client {




TileManager::TileManager( const shared_ptr<RequestService>& service )
	: mService(service)
{
	tnAssert(service);
}

TileManager::~TileManager()
{
}

shared_ptr<sf::Sprite> TileManager::getSpriteForStdTile(TNID tile_id ) const
{
	auto tileset_it = mIdToStdTileMapping.find(tile_id);

	if(tileset_it == mIdToStdTileMapping.end())
	{
		mService->requestStdTileName(tile_id);

		return nullptr;
	}
	
	return tileset_it->second;
}


void TileManager::identifyStdTile(const string& tile_name, TNID tile_id )
{

	auto it = mNameToStdTileMapping.find(tile_name);

	if(it == mNameToStdTileMapping.end())
	{
		// Tile not loaded!
		NOT_IMPLEMENTED();
	}

	mIdToStdTileMapping[tile_id] = it->second;
}

void TileManager::debug_load_test_tileset( const string& filename )
{
	Log loadLog(L"tile-load");

	loadLog.debug() << L"Load debug tileset from \"" << filename << "\"";

	std::wifstream file(lexical_convert<std::string>(filename));

	if(!file.is_open())
	{
		loadLog.error() << "Failed to load tile source!";
		return;
	}

	const auto& pool = StdTilePool::Inst();


	string line;
	int lineNum = 1;

	while(std::getline(file, line))
	{
		std::wistringstream iss(line);

		// skip comments
		if(line.size() && line.find(L"//") == string::npos)
		{
			string name;
			string std_tile_name;
			iss >> name >> std_tile_name;

			if(iss.fail() || !iss.eof())
			{
				loadLog.error() << L"Failed to parse line " << lineNum;
				return;
			}

			auto sprite = pool.getStdTile(std_tile_name);

			mNameToStdTileMapping[name] = sprite;
		}

		++lineNum;
	}

	loadLog.debug() << mNameToStdTileMapping.size() << " tiles loaded";
}





}
