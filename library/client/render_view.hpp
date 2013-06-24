#pragma once
#ifndef _RENDER_VIEW_HPP
#define _RENDER_VIEW_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

#include "settings.hpp"

namespace client {

class Bounds
{
public:
	Bounds();
	Bounds(const Vector& pos, const Vector& size);
	~Bounds();

	static Bounds center(const Bounds& outter, const Vector& rect);

	Vector	position;
	Vector	size;
};




class RenderView
{
public:
	RenderView(sf::RenderTarget& target, const Rect& tileSize);
	RenderView(RenderView& view, const Bounds& outter_bounds);
	RenderView(RenderView& view, const Bounds& outter_bounds, const Bounds& inner_bounds, const sf::Color& color);
	~RenderView();

	sf::RenderTarget& target();

	Bounds bounds() const;
	const sf::Color& color() const;

private:
	sf::FloatRect _calcViewport(const Bounds& outter, const Vector& topSize);
private:
	sf::RenderTarget& mTarget;
	sf::View mOldView;
	sf::Color mColor;
	Vector mSize;
};



}





#endif