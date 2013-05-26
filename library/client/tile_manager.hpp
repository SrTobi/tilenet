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
		virtual void requestStdIdTileName(TNID id) = 0;
	};


	class Tileset;
	class StdIdTileset;
public:
	TileManager(const shared_ptr<RequestService>& service);
	~TileManager();


	shared_ptr<sf::Sprite> getSpriteFromStdIdTileset(TNID tileset_id, TNID tile_id) const;


private:
	std::unordered_map<TNID, shared_ptr<Tileset>> mIdToTilesetMapping;
	std::unordered_map<string, shared_ptr<Tileset>> mNameToTilesetMapping;

	shared_ptr<RequestService> mService;
};



}






#endif