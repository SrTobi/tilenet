#pragma once
#ifndef _TILELAYER_HPP
#define _TILELAYER_HPP

#include <mutex>

#include "settings.hpp"

#include "layer.hpp"
#include "utils/field.hpp"

namespace net {
	class Message;
}


namespace srv {

class TileLayer
	: public Layer
{
public:
	TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags);
	~TileLayer();

	void putTile(const Point& pos, TNTILE* tile);

	const Ratio& ratio() const;
	const Rect&	size() const;

	OVERRIDE void destroy();
	OVERRIDE shared_ptr<TilenetObject> clone();
	OVERRIDE void flush();
	OVERRIDE shared_ptr<net::Message> getStateMessage();





private:
	std::mutex		mMutex;
	Field<TNTILE>	mTileField;
	const Ratio		mRatio;
};



}


#endif