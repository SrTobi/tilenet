#pragma once
#ifndef _RENDER_INTERFACE_HPP
#define _RENDER_INTERFACE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

#include "settings.hpp"



namespace client {


class RenderInterface
{
public:
	virtual void render(sf::RenderTarget& target) = 0;
};



}


#endif