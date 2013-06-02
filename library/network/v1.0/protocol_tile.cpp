#include "includes.hpp"
#include "protocol_tile.hpp"

#define CREATE_CONTAINER(_type)	auto* container = new TileDataContainer<_type>(); mData.reset(container); auto& data = container->data;

namespace net {


PTile::PTile()
	: mData(TNTILE::nullset_type())
{
}

PTile::PTile(const TNTILE& tile)
	: mData(TNTILE::nullset_type())
{
	assign(tile);
}


PTile::~PTile()
{
}


PTile::TileType PTile::type() const
{
	return TileType(mData.which());
}

PTile& PTile::operator=( const PTile& other )
{
	assign(other);
	return *this;
}

PTile& PTile::operator=( PTile&& other )
{
	assign(std::move(other));
	return *this;
}

PTile& PTile::operator=( const TNTILE& tile )
{
	assign(tile);
	return *this;
}

void PTile::assign( const PTile& other )
{
	mData = other.mData;
}

void PTile::assign( PTile&& other )
{
	mData = std::move(other.mData);
}

void PTile::assign(const TNTILE& tile)
{
	switch(tile.type)
	{
	case TN_NULL_TILE:
		mData = TNTILE::nullset_type();
		break;
	case TN_STD_TILE:
		mData = tile.data.stdset;
		break;
	case TN_CHAR_TILE:
		mData = tile.data.charset;
		break;
	default:
		IMPLEMENTATION_TODO("DO something, that this is reported to the user!");
		tnAssert(!"This is not a valid tile type!");
	}
}




}