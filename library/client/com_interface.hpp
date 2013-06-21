#pragma once
#ifndef _COM_INTERFACE_HPP
#define _COM_INTERFACE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "settings.hpp"



namespace client {


class ComInterface
{
public:
	virtual void notifyRender(sf::RenderTarget& target) = 0;
	virtual void notifyKeyevent(const sf::Event& event) = 0;
};



}


#endif