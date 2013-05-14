#include "includes.hpp"

#include <SFML/Window/Event.hpp>

#include "client_window.hpp"
#include "client.hpp"
#include "messenger.hpp"

namespace client {



ClientWindow::ClientWindow(const shared_ptr<ClientApp>& app, const shared_ptr<Messenger>& messenger)
	: mApp(app)
	, mMessenger(messenger)
{
}

ClientWindow::~ClientWindow()
{

}

void ClientWindow::init()
{
	mRenderWindow.create(sf::VideoMode(800, 600), "tilenet");

	if(!mRenderWindow.isOpen())
	{
		NOT_IMPLEMENTED();
	}
}

void ClientWindow::process()
{
	// Check for the window
	if(!mRenderWindow.isOpen())
	{
		auto app = mApp.lock();
		tnAssert(app);
		app->stop();
	}

	// Process event loop
	sf::Event event;
	while (mRenderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mRenderWindow.close();
	}

	mRenderWindow.clear();

	// If a renderer is set, then render
	if(mRenderer)
	{
		mRenderer->render(mRenderWindow);
	}



	mMessenger->render(mRenderWindow);
	mRenderWindow.display();
}

void ClientWindow::setLayerRenderer( const shared_ptr<RenderInterface>& renderer )
{
	mRenderer = renderer;
}



}