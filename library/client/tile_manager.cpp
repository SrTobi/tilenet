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
			mService->requestStdIdTileName(id);
			return nullptr;
		}

		return it->second;
	}



private:
	std::unordered_map<TNID, shared_ptr<sf::Sprite>> mIdToSpriteMapping;
	std::unordered_map<string, shared_ptr<sf::Sprite>> mNameToSpriteMapping;
	shared_ptr<RequestService> mService;
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





}