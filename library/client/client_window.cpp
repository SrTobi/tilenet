#include "includes.hpp"

#include "client_window.hpp"
#include "client.hpp"

namespace client {



ClientWindow::ClientWindow(const shared_ptr<ClientApp>& app)
	: mRenderWindow(new sf::RenderWindow())
	, mApp(app)
{
}

ClientWindow::~ClientWindow()
{

}

void ClientWindow::init()
{
	mRenderWindow->create(sf::VideoMode(800, 600), "tilenet");

	if(!mRenderWindow->isOpen())
	{
		NOT_IMPLEMENTED();
	}
}

void ClientWindow::process()
{
	// Check for the window
	if(!mRenderWindow->isOpen())
	{
		auto app = mApp.lock();
		tnAssert(app);
		app->stop();
	}

	// Process event loop
	sf::Event event;
	while (mRenderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mRenderWindow->close();
	}

	// If a renderer is set, then render
	if(mRenderer)
	{
		mRenderer->render();
	}
}

void ClientWindow::setLayerRenderer( const shared_ptr<RenderInterface>& renderer )
{
	mRenderer = renderer;
	mRenderer->init(mRenderWindow);
}



}