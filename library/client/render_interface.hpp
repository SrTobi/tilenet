#pragma once
#ifndef _RENDER_INTERFACE_HPP
#define _RENDER_INTERFACE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "settings.hpp"



namespace client {


class RenderInterface
{
public:
	virtual void init(const shared_ptr<sf::RenderTarget>& target) = 0;

	virtual void render() = 0;
};



}


#endif