#pragma once
#ifndef _CLIENT_WINDOW_HPP
#define _CLIENT_WINDOW_HPP

#include <SFML/Window.hpp>

#include "settings.hpp"
#include "render_interface.hpp"

namespace client {


class ClientApp;

class ClientWindow
{
public:
	ClientWindow(const shared_ptr<ClientApp>& app);
	~ClientWindow();

	void init();
	void process();

	void setLayerRenderer(const shared_ptr<RenderInterface>& renderer);

private:
	shared_ptr<sf::RenderWindow> mRenderWindow;
	shared_ptr<RenderInterface> mRenderer;

	weak_ptr<ClientApp> mApp;
};




}


#endif