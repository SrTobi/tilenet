#pragma once
#ifndef _CLIENT_WINDOW_HPP
#define _CLIENT_WINDOW_HPP

#include <SFML/Window.hpp>

#include "settings.hpp"

namespace client {


class ClientWindow
{
public:
	ClientWindow();
	~ClientWindow();

	void init();
	void process();

private:
	sf::RenderWindow mRenderWindow;
};




}


#endif