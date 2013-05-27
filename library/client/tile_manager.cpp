#include "includes.hpp"

#include <fstream>

#include "tile_manager.hpp"
#include "std_tile_pool.hpp"

namespace client {




class TileManager::Tileset
{
public:
	Tileset()
		: mTilesetId(0)
	{
	}

	virtual ~Tileset() {}


	void setId(TNID id)
	{
		tnAssert(mTilesetId == 0);
		mTilesetId = id;
	}

	TNID id() const
	{
		return mTilesetId;
	}

private:
	TNID mTilesetId;
};

class TileManager::StdIdTileset
	: public Tileset
{
public:
	StdIdTileset(const shared_ptr<RequestService>& service)
		: mService(service)
	{
	}
	
	~StdIdTileset()
	{
	}


	shared_ptr<sf::Sprite> getSpriteById(TNID tile_id) const
	{
		auto it = mIdToSpriteMapping.find(tile_id);

		if(it == mIdToSpriteMapping.end())
		{
			tnAssert(id());
			mService->requestStdIdTileName(tile_id, id());
			return nullptr;
		}

		return it->second;
	}

	void addTile(const string& name, const shared_ptr<sf::Sprite>& sprite)
	{
		tnAssert(sprite);
		mNameToSpriteMapping[name] = sprite;
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

void TileManager::identifyTileset( const string& tileset_name, TNID tileset_id )
{
	auto it = mNameToTilesetMapping.find(tileset_name);

	if(it == mNameToTilesetMapping.end())
	{
		NOT_IMPLEMENTED();
	}

	it->second->setId(tileset_id);
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
	Log loadLog(L"tile-load");

	loadLog.debug() << L"Load debug tileset from \"" << filename << "\"";

	std::wifstream file(filename);

	if(!file.is_open())
	{
		loadLog.error() << "Failed to load tile source!";
		return;
	}

	auto tileset = std::make_shared<StdIdTileset>(mService);
	mNameToTilesetMapping[L"#debug-std-tileset"] = tileset;


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

			tileset->addTile(name, sprite);
		}

		++lineNum;
	}

	loadLog.debug() << mNameToTilesetMapping.size() << " tiles loaded";
}





}