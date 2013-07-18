#include "includes.hpp"
#include "std_tile.hpp"


namespace client {







StdTile::StdTile( const sf::Sprite& sprite, const Texture& tex)
	: mSprite(sprite)
	, mTex(tex)
{
}

StdTile::~StdTile()
{
}

void StdTile::render( sf::RenderTarget& target, Point pos, const sf::Color& color )
{
	mSprite.setPosition(float(pos.x), float(pos.y));
	mSprite.setColor(color);
	target.draw(mSprite);
}

}