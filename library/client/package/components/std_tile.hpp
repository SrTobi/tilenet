#pragma once
#ifndef _STD_TILE_HPP
#define _STD_TILE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "settings.hpp"


namespace client {


class StdTile
{
public:
	virtual ~StdTile();
	virtual void render(sf::RenderTarget& target, Point pos, const sf::Color& color) = 0;
};



class StdSpriteTile : public StdTile
{
public:
	typedef shared_ptr<sf::Texture> Texture;

	StdSpriteTile(const sf::Sprite& sprite, const Texture& tex, const sf::Color& color);
	~StdSpriteTile();

	OVERRIDE virtual void render(sf::RenderTarget& target, Point pos, const sf::Color& color);

private:
	sf::Sprite mSprite;
	sf::Color mColor;
	Texture mTex;
};

class StdRefTile : public StdTile
{
public:

	StdRefTile(const shared_ptr<StdTile>& ref, const sf::Color& color);
	~StdRefTile();

	OVERRIDE virtual void render(sf::RenderTarget& target, Point pos, const sf::Color& color);
private:
	shared_ptr<StdTile> mRef;
	sf::Color mColor;
};



}




#endif