#include "includes.hpp"
#include "renderer_v1_0.hpp"

#include "utils/field.hpp"

#include "tile_mapper_v1_0.hpp"
#include "server_info_v1_0.hpp"

#include "client/package/package.hpp"
#include "client/package/package_manager.hpp"
#include "client/package/components/std_tile.hpp"


sf::Color to_sf_color(TNCOLOR c)
{
	sf::Color color;

	color.a = TNGET_ALPHA(c);
	color.r = TNGET_RED(c);
	color.g = TNGET_GREEN(c);
	color.b = TNGET_BLUE(c);

	return color;
}




namespace client{
namespace com {
namespace v1_0 {

class Renderer::Layer
{
public:

	virtual void render(sf::RenderTarget& target) = 0;
	virtual void putTile(const Point& pos, const net::PTile& ptile) = 0;
	virtual void getBounds(Rect& ratioSize, Rect& tileSize) const = 0;
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
		virtual void render(sf::RenderTarget& target, const Point& pos) = 0;
	};

	class StdIdTile
		: public Tile
	{
	public:
		StdIdTile(TNID tile_id, sf::Color color, const shared_ptr<TileMapper>& mapper)
			: mTileId(tile_id)
			, mTileMapper(mapper)
			, mColor(color)
		{
		}

		virtual OVERRIDE void render(sf::RenderTarget& target, const Point& pos)
		{
			std::shared_ptr<StdTile> tile = mTile.lock();

			if(!tile)
			{
				mTile = tile = mTileMapper->getStdTile(mTileId);
			}

			if(tile)
			{
				tile->render(target, pos, mColor);
			}
		}


	private:
		TNID mTileId;
		sf::Color mColor;
		std::weak_ptr<StdTile> mTile;
		shared_ptr<TileMapper> mTileMapper;
	};
public:
	RenderLayer(const Rect& size, const Ratio& ratio, const shared_ptr<TileMapper>& manager)
		: mRatio(ratio)
		, mTileManager(manager)
		, mTileField(size)
	{
	}

	OVERRIDE void render(sf::RenderTarget& target)
	{
		for(unsigned int x = 0; x < mTileField.size().w; ++x)
		{
			for(unsigned int y = 0; y < mTileField.size().h; ++y)
			{
				Point pos(x, y);
				auto& tile = mTileField.at(pos);

				if(tile)
					tile->render(target, pos);
			}
		}
	}

	virtual OVERRIDE void getBounds(Rect& ratioSize, Rect& tileSize) const
	{
		tileSize = mTileField.size();
	}

	virtual OVERRIDE void putTile(const Point& pos, const net::PTile& ptile)
	{
		auto& tile = mTileField.at(pos);

		switch(ptile.type())
		{
		case net::PTile::NullTileType:
			break;
		case net::PTile::StdTileType:
			{
				auto& data = ptile.data<TNTILE::stddata_type>();
				tile.reset(new StdIdTile(data.id, to_sf_color(data.color), mTileManager));
			}
			break;
		default:
			NOT_IMPLEMENTED();
		}
	}


private:
	Field<std::unique_ptr<Tile>> mTileField;
	Ratio mRatio;
	shared_ptr<TileMapper> mTileManager;
};









Renderer::Renderer(ClientWindow& window, const shared_ptr<TileMapper>& mapper, const shared_ptr<PackageManager>& pmanager, const shared_ptr<ServerInfo>& info)
	: mWindow(window)
	, mPackManager(pmanager)
	, mTileMapper(mapper)
	, mServerInfo(info)
{
}

Renderer::~Renderer()
{
}


void Renderer::render(sf::RenderTarget& target)
{
	// Set background
	target.clear(mBGCOlor);

	if(mPackage)
	{
		// Render layers
		auto topLayer = layer(mTopLayerId);


		if(topLayer)
		{
			sf::View view = target.getView();
			calculateView(target, topLayer);
			topLayer->render(target);
			target.setView(view);
		}
	}else{
		if(mPackManager->isSearching())
		{
			// Searching!!!
		}else{
			// No package loaded!!!
			mPackage = mPackManager->loadPackageByName(mServerInfo->packageName());

			if(!mPackage)
			{
				mPackage = mPackManager->loadPackageByInterface(mServerInfo->packageInterface());
			}

			if(!mPackage)
			{
				IMPLEMENTATION_TODO("Inform the user or download a package");
			}
		}
	}
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
	mIdToLayerMapping[id] = std::make_shared<RenderLayer>(size, r, mTileMapper);
}

void Renderer::putTile( TNID layerid, Point pos, const net::PTile& tile )
{
	auto it = mIdToLayerMapping.find(layerid);

	if(it == mIdToLayerMapping.end())
	{
		NOT_IMPLEMENTED();
	}

	it->second->putTile(pos, tile);
}

void Renderer::calculateView(sf::RenderTarget& target, const shared_ptr<Layer>& layer)
{
	Rect fieldSize;
	Rect ratioSize;

	sf::Vector2u win_size = target.getSize();

	layer->getBounds(ratioSize, fieldSize);
	
	Point center = fieldSize / 2;

	sf::View view(sf::FloatRect(0, 0, fieldSize.x, fieldSize.y));

	Vector rsize((float(fieldSize.x) * 8.0f) / float(win_size.x), (float(fieldSize.y) * 12.0f) / float(win_size.y));

	view.setViewport(sf::FloatRect(	0.5f - rsize.w / (2.0f),
									0.5f - rsize.h / (2.0f),
									rsize.w, rsize.h));

	target.setView(view);
}

}}}