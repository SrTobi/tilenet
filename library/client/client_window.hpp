#pragma once
#ifndef _CLIENT_WINDOW_HPP
#define _CLIENT_WINDOW_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "settings.hpp"
#include "render_interface.hpp"

namespace client {


class ClientApp;
class Messenger;

class ClientWindow
{
public:
	ClientWindow(ClientApp& app, const shared_ptr<Messenger>& messenger);
	~ClientWindow();

	void init();
	void process();

	void setLayerRenderer(const shared_ptr<RenderInterface>& renderer);

private:
	sf::RenderWindow mRenderWindow;
	shared_ptr<RenderInterface> mRenderer;
	shared_ptr<Messenger> mMessenger;

	ClientApp& mApp;
};




}


#endif