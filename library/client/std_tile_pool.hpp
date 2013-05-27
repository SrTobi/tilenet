#pragma once
#ifndef _STDTILEPOOL_H
#define _STDTILEPOOL_H

#include <unordered_map>


#include "settings.hpp"



namespace client {


class StdTilePool
{
public:
	StdTilePool();
	~StdTilePool();

	shared_ptr<sf::Sprite> getStdTile(const string& name) const;
	bool isStdTileName(const string& name);

	const sf::Texture& getTexture() const;

	static const StdTilePool& Inst();
private:
	void loadTextureFromSource(const sf::Uint8* src, unsigned int length);
	void addStdTile(unsigned int col, unsigned int row, const string& name);
	void init();

	std::unordered_map<string, shared_ptr<sf::Sprite> > mTilesAssociation;
	sf::Texture mTileImage;


	static const unsigned int	StdTileImageSourceLegth;
	static const byte			StdTileImageSource[];
};





}




#endif