#include "includes.hpp"
#include "renderer_v1_0.hpp"

#include "client/tile_manager.hpp"

namespace client{
namespace com {
namespace v1_0 {

class Renderer::Layer
{
public:

	virtual void render(sf::RenderTarget& target) = 0;

private:
};


class Renderer::RenderLayer
	: public Layer
{
private:
	class Tile
	{
	public:
		virtual ~Tile() {}
		virtual void render(sf::RenderTarget& target) = 0;
	};

	class StdIdTile
		: public Tile
	{
	public:
		StdIdTile(TNID tile_id, TNID tileset_id, sf::Color color, const shared_ptr<TileManager>& manager)
			: mTileId(tile_id)
			, mTilesetId(tileset_id)
			, mTileManager(manager)
			, mColor(color)
		{
		}

		virtual OVERRIDE void render(sf::RenderTarget& target)
		{
			std::shared_ptr<sf::Sprite> sprite = mSprite.lock();

			if(!sprite)
			{
				mSprite = sprite = mTileManager->getSpriteFromStdIdTileset(mTilesetId, mTileId);
			}

			if(sprite)
			{
				sprite->setColor(mColor);
				target.draw(*sprite);
			}
		}


	private:
		TNID mTileId, mTilesetId;
		sf::Color mColor;
		std::weak_ptr<sf::Sprite> mSprite;
		shared_ptr<TileManager> mTileManager;
	};
public:
	RenderLayer(const Rect& size, const Ratio& ratio, const shared_ptr<TileManager>& manager)
		: mSize(size)
		, mRatio(ratio)
		, mTileManager(manager)
	{
	}

	OVERRIDE void render(sf::RenderTarget& target)
	{

	}

private:
	Rect mSize;
	Ratio mRatio;
	shared_ptr<TileManager> mTileManager;
};









Renderer::Renderer( const shared_ptr<ClientWindow>& window, const shared_ptr<TileManager>& manager)
	: mWindow(window)
	, mTileManager(manager)
{
}

Renderer::~Renderer()
{
}


void Renderer::render(sf::RenderTarget& target)
{
	// Set background
	target.clear(mBGCOlor);

	// Render layers
	auto topLayer = layer(mTopLayerId);

	if(topLayer)
		topLayer->render(target);
}


void Renderer::setTopLayer( TNID id )
{
	mTopLayerId = id;

	IMPLEMENTATION_TODO("Clear all layers or something intelligent!");
}

shared_ptr<Renderer::Layer> Renderer::layer( TNID id ) const
{
	auto it = mIdToLayerMapping.find(id);

	if(it == mIdToLayerMapping.end())
		return shared_ptr<Layer>();

	return it->second;
}

void Renderer::defineLayer( TNID id, Rect size, Ratio r )
{
	mIdToLayerMapping[id] = std::make_shared<RenderLayer>(size, r, mTileManager);
}

void Renderer::putTile( TNID layerid, Point pos, const net::PTile& tile )
{

}

}}}