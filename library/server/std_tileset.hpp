#pragma once
#ifndef _STD_TILESET_HPP
#define _STD_TILESET_HPP



#include <mutex>
#include <unordered_map>
#include "settings.hpp"


namespace srv {

class StdTileset
	: boost::noncopyable
{
public:
	StdTileset();
	~StdTileset();

	TNID registerTile(const string& name);

	const string& getTileName(TNID id) const;

	static StdTileset& Inst();

private:
	mutable std::mutex mMutex;
	std::unordered_map<TNID, string> mTiles;
	TNID mNextId;
};







}

#endif