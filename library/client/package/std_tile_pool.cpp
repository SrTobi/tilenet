#include "includes.hpp"
#include "std_tile_pool.hpp"
#include "components/std_tile.hpp"


namespace client {


const unsigned int TILE_WIDTH = 8;
const unsigned int TILE_HEIGHT = 12;

#include "std_tile_set.inl"


StdTilePool::StdTilePool()
{
	mTileImage = std::make_shared<sf::Texture>();
	init();
}

StdTilePool::~StdTilePool()
{

}

shared_ptr<StdTile> StdTilePool::getStdTile( const string& name ) const
{
	auto it = mTilesAssociation.find(name);

	if(it == mTilesAssociation.end())
		return nullptr;

	return it->second;
}

bool StdTilePool::isStdTileName( const string& name )
{
	return mTilesAssociation.find(name) != mTilesAssociation.end();
}

const StdTilePool& StdTilePool::Inst()
{
	static StdTilePool pool;
	return pool;
}

/*
 *	src enthält die daten
 *	size enthaelt die anzahl der zeichen
 *
 *	Aufbau:
 *		Immer 12 bytes geben ein Zeichen. die bits bilden jeweils die reihen
 *
 *
 */
void StdTilePool::loadTextureFromSource( const sf::Uint8* src, unsigned int length)
{

	unsigned int imgSize = 16;
	while((imgSize / TILE_WIDTH) * (imgSize / TILE_HEIGHT) < length)
	{
		imgSize *= 2;
	}

	sf::Image img;
	img.create(imgSize, imgSize, sf::Color::Transparent);

	const unsigned int tilesPerRow = imgSize / TILE_WIDTH;
	const unsigned int tileRows = imgSize / TILE_HEIGHT;

	unsigned int tilesExtracted = 0;

	for(unsigned int row = 0; row < tileRows; ++row)
	{
		for(unsigned int col = 0; col < tilesPerRow; ++col)
		{
			if(tilesExtracted >= length)
			{
				mTileImage->loadFromImage(img);
				return;
			}

			const unsigned int ybegin = row * TILE_HEIGHT;
			const unsigned int yend = ybegin + TILE_HEIGHT;

			for(unsigned int y = ybegin; y < yend; ++y)
			{
				byte rowContent = *src; ++src;
				const unsigned int xbegin = col * TILE_WIDTH;
				
				for(unsigned int x = xbegin; rowContent; ++x)
				{
					if(rowContent & 0x80)
					{
						img.setPixel(x,y, sf::Color::White);
					}
					rowContent <<= 1;
				}
			}
			++tilesExtracted;
		}
	}
}

void StdTilePool::addStdTile( unsigned int col, unsigned int row, const string& names )
{
	auto sprite = sf::Sprite(*mTileImage, sf::IntRect(col * TILE_WIDTH, row * TILE_HEIGHT, /*(col+1) **/ TILE_WIDTH, /*(row+1) **/ TILE_HEIGHT));
	auto tile = std::make_shared<StdTile>(sprite, mTileImage);

	std::wistringstream iss(names);
	string name;
	bool first = true;

	while(std::getline(iss, name, L'|'))
	{
		if(name.empty())
			name = L"|";
		bool newInserted;
		std::tie(std::ignore, newInserted) = mTilesAssociation.insert(std::make_pair(name, tile));
		tnAssert(newInserted);
	}
}


void StdTilePool::init()
{
	loadTextureFromSource(StdTileImageSource, StdTileImageSourceLegth);

	// space tile
	addStdTile(0, 8, L" |space");


	// numbers
	addStdTile(6, 1, L"0|zero");
	addStdTile(7, 1, L"1|one");
	addStdTile(8, 1, L"2|two");
	addStdTile(9, 1, L"3|three");
	addStdTile(10, 1, L"4|four");
	addStdTile(11, 1, L"5|five");
	addStdTile(12, 1, L"6|six");
	addStdTile(13, 1, L"7|seven");
	addStdTile(14, 1, L"8|eight");
	addStdTile(15, 1, L"9|nine");

	// upper case
	addStdTile(23, 1, L"A");
	addStdTile(24, 1, L"B");
	addStdTile(25, 1, L"C");
	addStdTile(26, 1, L"D");
	addStdTile(27, 1, L"E");
	addStdTile(28, 1, L"F");
	addStdTile(29, 1, L"G");
	addStdTile(30, 1, L"H");
	addStdTile(31, 1, L"I");
	addStdTile(0, 2, L"J");
	addStdTile(1, 2, L"K");
	addStdTile(2, 2, L"L");
	addStdTile(3, 2, L"M");
	addStdTile(4, 2, L"N");
	addStdTile(5, 2, L"O");
	addStdTile(6, 2, L"P");
	addStdTile(7, 2, L"Q");
	addStdTile(8, 2, L"R");
	addStdTile(9, 2, L"S");
	addStdTile(10, 2, L"T");
	addStdTile(11, 2, L"U");
	addStdTile(12, 2, L"V");
	addStdTile(13, 2, L"W");
	addStdTile(14, 2, L"X");
	addStdTile(15, 2, L"Y");
	addStdTile(16, 2, L"Z");

	// lower case
	addStdTile(23, 2, L"a");
	addStdTile(24, 2, L"b");
	addStdTile(25, 2, L"c");
	addStdTile(26, 2, L"d");
	addStdTile(27, 2, L"e");
	addStdTile(28, 2, L"f");
	addStdTile(29, 2, L"g");
	addStdTile(30, 2, L"h");
	addStdTile(31, 2, L"i");
	addStdTile(0, 3, L"j");
	addStdTile(1, 3, L"k");
	addStdTile(2, 3, L"l");
	addStdTile(3, 3, L"m");
	addStdTile(4, 3, L"n");
	addStdTile(5, 3, L"o");
	addStdTile(6, 3, L"p");
	addStdTile(7, 3, L"q");
	addStdTile(8, 3, L"r");
	addStdTile(9, 3, L"s");
	addStdTile(10, 3, L"t");
	addStdTile(11, 3, L"u");
	addStdTile(12, 3, L"v");
	addStdTile(13, 3, L"w");
	addStdTile(14, 3, L"x");
	addStdTile(15, 3, L"y");
	addStdTile(16, 3, L"z");

	// smiley
	addStdTile(0, 0, L"smiley|smiley1|dark-smiley");
	addStdTile(0, 1, L"smiley2|light-smiley");
	
	// punctuations
	addStdTile(4, 1, L".|fullstop|point|dot");
	addStdTile(2, 1, L",|comma");
	addStdTile(16, 1, L":|colon");
	addStdTile(17, 1, L";|semicolon");
	addStdTile(21, 1, L"?|question|questionmark");

	// math
	addStdTile(3, 1, L"-|dash|minus");
	addStdTile(1, 1, L"+|plus|add");
	addStdTile(0, 1, L"*|mul|mult|asterisk");
	addStdTile(5, 1, L"/|slash|divide|div");
	addStdTile(19, 1, L"=|equals");
	addStdTile(18, 1, L"<|less-than");
	addStdTile(20, 1, L">|greater-than");

	// stuff
	addStdTile(18, 3, L"|vbar|vertical-bar");
}

const std::shared_ptr<sf::Texture>& StdTilePool::getTexture() const
{
	return mTileImage;
}










}