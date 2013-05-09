#include "includes.hpp"

#include "tile.hpp"
#include "server/tileset.hpp"



namespace srv {

const shared_ptr<Tileset> Tile::NullTilesetPtr;


TileBase::TileBase(const shared_ptr<Tileset>& tileset)
	: mTileset(tileset)
{
}

TileBase::TileBase( const TileBase& other )
	: mTileset(other.mTileset)
{
}

TileBase::~TileBase()
{
}


const shared_ptr<Tileset>& TileBase::tileset() const
{
	return mTileset;
}







Tile::Tile()
{
}

Tile::Tile( const Tile& other )
{
	assign(other);
}

Tile::Tile( const Tile&& other )
{
	assign(other);
}

Tile::~Tile()
{
}

Tile& Tile::operator=( const Tile& other )
{
	assign(other);
	return *this;
}

Tile& Tile::operator=(Tile&& other )
{
	assign(other);
	return *this;
}

bool Tile::assign( const Tile& other )
{
	if(isNull())
	{
		if(other.isNull())
		{
			return false;
		}

		mTile = other.mTile->clone();
		return true;
	}else{
		
		if(other.isNull())
		{
			mTile.reset();
			return true;
		}

		if(other.tileset() == tileset())
		{
			NOT_IMPLEMENTED();
		}else{
			mTile = other.mTile->clone();
			return true;
		}

	}
}

bool Tile::assign(Tile&& other)
{
	if(isNull())
	{
		if(other.isNull())
		{
			return false;
		}
		mTile = std::move(other.mTile);
		
		return true;
	}else{

		if(other.isNull())
		{
			mTile.reset();
			return true;
		}

		if(other.tileset() == tileset())
		{
			NOT_IMPLEMENTED();
		}else{
			mTile = std::move(other.mTile);
			return true;
		}
	}
}

bool Tile::isNull() const
{
	return mTile;
}

const shared_ptr<Tileset>& Tile::tileset() const
{
	return mTile? mTile->tileset() : NullTilesetPtr;
}

void Tile::clear()
{
	mTile.reset();
}

srv::Tile Tile::NullTile()
{
	return Tile();
}




}