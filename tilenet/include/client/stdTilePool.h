#pragma once
#ifndef _STDTILEPOOL_H
#define _STDTILEPOOL_H



#include "settings.h"



namespace client {


class StdTilePool
{
public:
	StdTilePool();
	~StdTilePool();

	sf::Sprite* getStdTile(const wstring& name);
	bool isStdTileName(const wstring& name);

	const sf::Texture& getTexture() const;

	static const StdTilePool& Inst();
private:
	void loadTextureFromSource(const sf::Uint8* src, uint length);
	void addStdTile(uint col, uint row, const wstring& name);
	void init();

	std::map<wstring, shared_ptr<sf::Sprite> > mTilesAssociation;
	sf::Texture mTileImage;


	static const uint	StdTileImageSourceLegth;
	static const byte	StdTileImageSource[];
};





}




#endif
