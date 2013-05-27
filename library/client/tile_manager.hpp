#pragma once
#ifndef _TILE_MANAGER_HPP
#define _TILE_MANAGER_HPP


#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>

#include "settings.hpp"



namespace client {


class TileManager
{
public:
	struct RequestService
	{
		virtual void requestTilesetName(TNID id) = 0;
		virtual void requestStdIdTileName(TNID tile_id, TNID tileset_id) = 0;
	};


	class Tileset;
	class StdIdTileset;
public:
	TileManager(const shared_ptr<RequestService>& service);
	~TileManager();

	void debug_load_test_tileset(const string& filename);

	shared_ptr<sf::Sprite> getSpriteFromStdIdTileset(TNID tileset_id, TNID tile_id) const;
	
	void identifyTileset(const string& tileset_name, TNID tileset_id);
	void identifyStdIdTile(TNID tileset_id, const string& tile_name, TNID tile_id);

private:
	std::unordered_map<TNID, shared_ptr<Tileset>> mIdToTilesetMapping;
	std::unordered_map<string, shared_ptr<Tileset>> mNameToTilesetMapping;

	shared_ptr<RequestService> mService;
};



}






#endif