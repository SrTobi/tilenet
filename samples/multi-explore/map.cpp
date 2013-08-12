#include "map.hpp"
#include <ctime>







Map::Map( const tiley::Rect& size )
	: mField(size)
{
	srand(time(nullptr));
	mField.clear(Plain);

	// Set some trees
	for(int i = 0; i < size.area() / 10; ++i)
	{
		mField.at(freeRandPos()) = Tree;
	}
}

Map::~Map()
{

}

Map::TileType Map::tile( const tiley::Point& p )
{
	if(p.x < 0 || p.x >= size().w || p.y < 0 || p.y >= size().h)
		return Empty;

	return mField.at(p);
}

void Map::togglePlayer( const tiley::Point& p )
{
	assert(p.x >= 0 || p.x < size().w || p.y >= 0 || p.y < size().h);
	auto& tile = mField.at(p);

	tile = ((tile == Player)? Plain : Player);
}

tiley::Point Map::freeRandPos() const
{
	tiley::Point p;

	do 
	{
		p.x = rand() % size().w;
		p.y = rand() % size().h;

	} while (mField.at(p) != Plain);

	return p;
}
