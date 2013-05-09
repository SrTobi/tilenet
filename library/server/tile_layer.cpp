#include "includes.hpp"
#include "server/tile_layer.hpp"


namespace srv {


TileLayer::TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags)
	: mTileField(size)
	, mRatio(ratio)
{
}

TileLayer::~TileLayer()
{
}

void TileLayer::putTile(const Point& pos, TNTILE* tile)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mTileField.at(pos) = *tile;
}

const Ratio& TileLayer::ratio() const
{
	return mRatio;
}

const Rect& TileLayer::size() const
{
	return mTileField.size();
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