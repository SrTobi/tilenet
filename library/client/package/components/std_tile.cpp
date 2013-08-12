#include "includes.hpp"
#include "std_tile.hpp"

#include <numeric>
#include <random>

namespace client {



/**************************************************   StdTile   **************************************************/
StdTile::~StdTile()
{

}






/**************************************************   StdSpriteTile   **************************************************/
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
	mSprite.setColor(mColor * color);
	target.draw(mSprite);
}





/**************************************************   StdRefTile   **************************************************/
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
	if(mRef)
		mRef->render(target, pos, color * mColor);
}


/**************************************************   StdAnimationTile   **************************************************/
StdAnimationTile::StdAnimationTile( unsigned int interval, std::vector<shared_ptr<StdTile>>&& tiles, std::vector<unsigned int>&& portions, bool rand)
	: mInterval(interval)
	, mTiles(std::move(tiles))
	, mRand(rand)
	, mLastEpochTime(0)
{
	while(portions.size() < mTiles.size())
		portions.push_back(1);

	float total = std::accumulate(portions.begin(), portions.end(), 0.0f);

	tnAssert(total > 0);

#pragma warning(disable: 4244)
	for(float p : portions)
	{
		mPortions.push_back(p / total);
	}

	portions.clear();
}

StdAnimationTile::~StdAnimationTile()
{
}


OVERRIDE void StdAnimationTile::render( sf::RenderTarget& target, Point pos, const sf::Color& color )
{
	using namespace std::chrono;
	auto epochtime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	
	// prevent equal animations from being rendered differently in the same frame
	if(mLastEpochTime <= epochtime && epochtime <= mLastEpochTime + 2)
	{
		epochtime = mLastEpochTime;
	}else
		mLastEpochTime = epochtime;

	// render the tile depending on its position
	if(mRand)
	{
		std::mt19937 rg;
		rg.seed(1 + pos.x * 100 + pos.y * 100000);
		epochtime += rg();
	}

	float p = float(epochtime % mInterval) / float(mInterval);

	for(unsigned int i = 0; i < mPortions.size(); ++i)
	{
		p -= mPortions[i];

		if(p <= 0.0f)
		{
			mTiles[i % mTiles.size()]->render(target, pos, color);
			return;
		}
	}
}


}