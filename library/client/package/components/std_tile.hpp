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
	StdTile(const sf::Sprite& sprite);
	~StdTile();

	void render(sf::RenderTarget& target, Point pos, const sf::Color& color);

private:
	sf::Sprite mSprite;
};





}




#endif