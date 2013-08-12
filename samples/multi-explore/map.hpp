#pragma once
#ifndef _MULTI_EXPLORE_MAP_HPP
#define _MULTI_EXPLORE_MAP_HPP


#include <vector>
#include <tiley/field2d.hpp>

class Map
{
public:
	enum TileType
	{
		Empty,
		Plain,
		Tree,
		Water,
		Player
	};
public:
	Map(const tiley::Rect& size);
	~Map();

	void togglePlayer(const tiley::Point& p);
	TileType tile(const tiley::Point& p);
	tiley::Point freeRandPos() const;

	inline const tiley::Rect& size() const { return mField.size(); }
private:
	tiley::Field2D<TileType> mField;
};


#endif