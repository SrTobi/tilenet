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
	typedef shared_ptr<sf::Texture> Texture;

	StdTile(const sf::Sprite& sprite, const Texture& tex);
	~StdTile();

	void render(sf::RenderTarget& target, Point pos, const sf::Color& color);

private:
	sf::Sprite mSprite;
	Texture mTex;
};





}




#endif