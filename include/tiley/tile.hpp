#pragma once
#ifndef _TILEY_TILE_HPP
#define _TILEY_TILE_HPP

#include <type_traits>
#include <cassert>
#include "tilenet.h"

namespace tiley {

template<bool Const = true>
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

	template<bool Const>
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



class StdTile
{
public:
	template<typename Ch>
	StdTile(const std::basic_string<Ch>& name)
	{
		id = Impl::Stdtile(name);
	}

	TNTILE operator ()() const
	{
		return (*this)(0xFFFFFFFF);
	}

	TNTILE operator ()(TNCOLOR color) const
	{
		TNTILE tile;
		tile.type = TN_STD_TILE;
		tile.data.stdset.id = id;
		tile.data.stdset.color = color;
		tile.data.stdset.modifier = 0;

		return tile;
	}

private:
	TNID id;
};

}




#endif