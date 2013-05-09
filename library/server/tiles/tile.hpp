#pragma once
#ifndef _TILE_HPP
#define _TILE_HPP

#include "settings.hpp"



namespace srv {

class Tileset;


class TileBase
{
public:
	TileBase(const shared_ptr<Tileset>& tileset);
	TileBase(const TileBase& other);
	~TileBase();

	virtual std::unique_ptr<TileBase> clone() const = 0;

	const shared_ptr<Tileset>& tileset() const;

private:
	const shared_ptr<Tileset> mTileset;
};



class Tile
{
public:
	Tile();
	Tile(const Tile& other);
	Tile(const Tile&& other);
	~Tile();

	Tile& operator =(const Tile& other);
	Tile& operator =(Tile&& other);

	// return if something has changed
	bool assign(const Tile& other);
	bool assign(Tile&& other);

	bool isNull() const;
	const shared_ptr<Tileset>& tileset() const;

	void clear();

	static Tile NullTile();
private:
	static const shared_ptr<Tileset> NullTilesetPtr;
	mutable std::unique_ptr<TileBase> mTile;
};



}





#endif