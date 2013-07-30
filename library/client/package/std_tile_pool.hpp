#pragma once
#ifndef _STDTILEPOOL_H
#define _STDTILEPOOL_H

#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "settings.hpp"



namespace client {

class StdTile;

class StdTilePool
{
public:
	StdTilePool();
	~StdTilePool();

	shared_ptr<StdTile> getStdTile(const string& name) const;
	bool isStdTileName(const string& name);

	const shared_ptr<sf::Texture>& getTexture() const;

	static Rect GetTileSize();
	static const StdTilePool& Inst();
private:
	void loadTextureFromSource(const sf::Uint8* src, unsigned int length);
	void addStdTile(unsigned int col, unsigned int row, const string& name);
	void init();

	std::unordered_map<string, shared_ptr<StdTile> > mTilesAssociation;
	std::shared_ptr<sf::Texture> mTileImage;


	static const unsigned int	StdTileImageSourceLegth;
	static const byte			StdTileImageSource[];
};





}




#endif