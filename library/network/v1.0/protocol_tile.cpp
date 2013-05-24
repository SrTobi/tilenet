#include "includes.hpp"
#include "protocol_tile.hpp"

#define CREATE_CONTAINER(_type)	auto* container = new TileDataContainer<_type>(); mData.reset(container); auto& data = container->data;

namespace net {


PTile::PTile()
{
}

PTile::PTile( TNID tileset_id, TNID tile_id, TNFLAG modifier, TNCOLOR color )
{
	CREATE_CONTAINER(StdTileData);

	data.tileset_id = tileset_id;
	data.tile_id = tile_id;
	data.modifier = modifier;
	data.color = color;
}

PTile::PTile( TNID tileset_id, wchar_t c, TNFLAG modifier, TNCOLOR color )
{

	CREATE_CONTAINER(CharTileData);

	data.tileset_id = tileset_id;
	data.character = c;
	data.modifier = modifier;
	data.color = color;
}

PTile::PTile( const PTile& other )
{
	assign(other);
}

PTile::PTile( PTile&& other )
{
	assign(other);
}

PTile::~PTile()
{
}


PTile::TileType PTile::type() const
{
	if(!mData)
		return NullTileType;
	return mData->type();
}

PTile& PTile::operator=( const PTile& other )
{
	assign(other);
	return *this;
}

PTile& PTile::operator=( const PTile&& other )
{
	assign(other);
	return *this;
}

void PTile::assign( const PTile& other )
{
	mData = other.mData->clone();
}

void PTile::assign( PTile&& other )
{
	mData = std::move(other.mData);
}




}