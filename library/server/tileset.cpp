#include "includes.hpp"
#include "server/tileset.hpp"


namespace srv {
	

Tileset::Tileset( const string& name, TNFLAG flags )
	: mName(name)
	, mFlags(flags)
{
}

Tileset::~Tileset()
{
}

void Tileset::destroy()
{
	NOT_IMPLEMENTED();
}

shared_ptr<TilenetObject> Tileset::clone()
{
	NOT_IMPLEMENTED();
}

void Tileset::flush()
{
	NOT_IMPLEMENTED();
}

const string& Tileset::name() const
{
	return mName;
}



}