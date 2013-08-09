#include "includes.hpp"
#include "std_tile.hpp"


namespace client {



StdTile::~StdTile()
{

}






StdSpriteTile::StdSpriteTile( const sf::Sprite& sprite, const Texture& tex, const sf::Color& color)
	: mSprite(sprite)
	, mTex(tex)
	, mColor(color)
{
	// Do scaling
	mSprite.setScale(1.0f / float(sprite.getTextureRect().width), 1.0f / float(sprite.getTextureRect().height));
}

StdSpriteTile::~StdSpriteTile()
{
}

OVERRIDE void StdSpriteTile::render( sf::RenderTarget& target, Point pos, const sf::Color& color )
{
	mSprite.setPosition(float(pos.x), float(pos.y));
	mSprite.setColor(color);
	target.draw(mSprite);
}





StdRefTile::StdRefTile( const shared_ptr<StdTile>& ref, const sf::Color& color )
	: mRef(ref)
	, mColor(color)
{
}

StdRefTile::~StdRefTile()
{
}

OVERRIDE void StdRefTile::render(sf::RenderTarget& target, Point pos, const sf::Color& color)
{
	mRef->render(target, pos, color * mColor);
}


}