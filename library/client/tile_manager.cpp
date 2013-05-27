#include "includes.hpp"
#include "tile_manager.hpp"


namespace client {




class TileManager::Tileset
{
public:
	virtual ~Tileset() {}
};

class TileManager::StdIdTileset
{
public:
	StdIdTileset(const shared_ptr<RequestService>& service)
		: mService(service)
		, mTilesetId(0)
	{
	}
	
	~StdIdTileset()
	{
	}


	shared_ptr<sf::Sprite> getSpriteById(TNID id) const
	{
		auto it = mIdToSpriteMapping.find(id);

		if(it == mIdToSpriteMapping.end())
		{
			tnAssert(mTilesetId);
			mService->requestStdIdTileName(id, mTilesetId);
			return nullptr;
		}

		return it->second;
	}

	void identifyTile(const string& name, TNID id)
	{
		auto it = mNameToSpriteMapping.find(name);

		if(it == mNameToSpriteMapping.end())
		{
			NOT_IMPLEMENTED();
		}

		mIdToSpriteMapping[id] = it->second;
	}

	void setId(TNID id)
	{
		mTilesetId = id;
	}


private:
	std::unordered_map<TNID, shared_ptr<sf::Sprite>> mIdToSpriteMapping;
	std::unordered_map<string, shared_ptr<sf::Sprite>> mNameToSpriteMapping;
	shared_ptr<RequestService> mService;
	TNID mTilesetId;
};








TileManager::TileManager( const shared_ptr<RequestService>& service )
	: mService(service)
{
	tnAssert(service);
}

TileManager::~TileManager()
{
}

shared_ptr<sf::Sprite> TileManager::getSpriteFromStdIdTileset( TNID tileset_id, TNID tile_id ) const
{
	auto tileset_it = mIdToTilesetMapping.find(tileset_id);

	if(tileset_it == mIdToTilesetMapping.end())
	{
		mService->requestTilesetName(tileset_id);

		return nullptr;
	}
	
	auto tileset = std::dynamic_pointer_cast<const StdIdTileset>(tileset_it->second);
	
	if(!tileset)
	{
		NOT_IMPLEMENTED();
	}

	return tileset->getSpriteById(tile_id);
}

void TileManager::identifyTileset( const string& tileset_name, TNID tileset_id )
{
	auto it = mNameToTilesetMapping.find(tileset_name);

	if(it == mNameToTilesetMapping.end())
	{
		NOT_IMPLEMENTED();
	}

	mIdToTilesetMapping[tileset_id] = it->second;
}

void TileManager::identifyStdIdTile( TNID tileset_id, const string& tile_name, TNID tile_id )
{

	auto it = mIdToTilesetMapping.find(tileset_id);

	if(it == mIdToTilesetMapping.end())
	{
		NOT_IMPLEMENTED();
	}

	auto tileset = std::dynamic_pointer_cast<StdIdTileset>(it->second);

	if(!tileset)
	{
		NOT_IMPLEMENTED();
	}

	tileset->identifyTile(tile_name, tile_id);

}

void TileManager::debug_load_test_tileset( const string& filename )
{
	NOT_IMPLEMENTED();
}





}