#pragma once
#ifndef _CLIENT_LAYER_H
#define _CLIENT_LAYER_H


#include "settings.h"

namespace client{
namespace v1 {

class ClientLogic;

class ClientLayer
	: public boost::enable_shared_from_this<ClientLayer>
{
public:
	typedef std::vector< std::vector< sf::Sprite > > Field;
public:
	ClientLayer(nuint id, const Point& position, const Rect& size, const shared_ptr<ClientLogic>& logic);
	~ClientLayer();

	sf::Sprite* tile(const Point& pos);

	void updateProperties(const Point& position, const Rect& size);
	void setTile(const Point& pos);
	void setColor(const Point& color);

	void setPosition(const Point& pos);
	void setSize(const Rect& size);

	void render(sf::RenderTarget& target);

	nuint id() const { return mId; }

	Point position() const { return mPosition; }
	Rect size() const { return Rect(mField.size(), mField[0].size()); }
private:
	nuint mId;
	Point mPosition;
	Field mField;

	weak_ptr<ClientLogic>	mLogic;
};



}}

#endif
