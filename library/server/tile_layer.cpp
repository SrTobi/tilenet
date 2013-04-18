#include "includes.hpp"
#include "server/tile_layer.hpp"


namespace srv {


TileLayer::TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags)
{
	NOT_IMPLEMENTED();
}

TileLayer::~TileLayer()
{
	NOT_IMPLEMENTED();
}

void TileLayer::putTile(const Point& pos, TNTILE* tile)
{
	NOT_IMPLEMENTED();
}

const Ratio& TileLayer::ratio() const
{
	NOT_IMPLEMENTED();
}

const Rect& TileLayer::size() const
{
	NOT_IMPLEMENTED();
}

OVERRIDE void TileLayer::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> TileLayer::clone()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void TileLayer::flush()
{
	NOT_IMPLEMENTED();
}

shared_ptr<net::Message> TileLayer::getStateMessage()
{
	NOT_IMPLEMENTED();
}


}