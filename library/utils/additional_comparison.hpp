#pragma once
#ifndef _ADDITIONAL_COMPARISON_HPP
#define _ADDITIONAL_COMPARISON_HPP


#include "tilenet.h"
#include "settings.hpp"


#define _TN_CMP(_member) ((t1._member) == (t2._member))
inline bool operator ==(const TilenetNullData& t1, const TilenetNullData& t2)
{
	return true;
}


inline bool operator ==(const TilenetStdTilesetData& t1, const TilenetStdTilesetData& t2)
{
	return _TN_CMP(id) && _TN_CMP(color) && _TN_CMP(modifier);
}


inline bool operator ==(const TilenetCharTilesetData& t1, const TilenetCharTilesetData& t2)
{
	return _TN_CMP(ch) && _TN_CMP(color) && _TN_CMP(modifier);
}






#endif
