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
		virtual void requestStdTileName(TNID tile_id) = 0;
	};


	class Tileset;
	class StdIdTileset;
public:
	TileManager(const shared_ptr<RequestService>& service);
	~TileManager();

	void debug_load_test_tileset(const string& filename);

	shared_ptr<sf::Sprite> getSpriteForStdTile(TNID tile_id) const;
	
	void identifyStdTile(const string& tile_name, TNID tile_id);

private:
	std::unordered_map<TNID, shared_ptr<sf::Sprite>> mIdToStdTileMapping;
	std::unordered_map<string, shared_ptr<sf::Sprite>> mNameToStdTileMapping;

	shared_ptr<RequestService> mService;
};



}






#endif