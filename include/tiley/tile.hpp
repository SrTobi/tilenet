#pragma once
#ifndef _TILEY_TILE_HPP
#define _TILEY_TILE_HPP

#include <type_traits>
#include <cassert>
#include "tilenet.h"

namespace tiley {

template<typename Const = true>
class ITile;

class Tile
{
public:
	enum TileType
	{
		NullTile = TN_NULL_TILE,
		StdTile = TN_STD_TILE,
		CharTile = TN_CHAR_TILE
	};
public:
	inline Tile()
	{
		clear();
	}
	
	inline Tile(const TNTILE& raw)
		: mTile(raw)
	{
	}

	template<typename Const>
	inline Tile(const ITile<Const>& other);

	inline Tile(const Tile& other)
		: mTile(other.mTile)
	{

	}

	~Tile();

	inline void clear()
	{
		mTile.type = TNTILIETYPE(NullTile);
	}

	inline TileType type() const
	{
		return TileType(mTile.type);
	}

	template<typename T>
	inline const T as() const
	{
		return T(mTile);
	}


private:
	TNTILE mTile;
};

template<bool Const, Tile::TileType Type>
class ITile
{
public:
	typedef typename std::conditional<Const, const TNTILE, TNTILE>::type value_type;


	ITile(value_type& tile)
		: mTileRef(&tile)
	{
	}

	template<typename C>
	ITile(const ITile<C>& other)
		: mTileRef(other.mTileRef)
	{
		assert(mTileRef);
	}

	Tile::TileType type() const
	{
		return Type;
	}

protected:
	inline value_type& ref()
	{
		assert(mTileRef);
		return *mTileRef;
	}

private:
	static TNTILE EmptyTile;
	value_type* mTileRef;
};

template<typename Const>
TNTILE ITile::EmptyTile = {TN_NULL_TILE};


}




#endif