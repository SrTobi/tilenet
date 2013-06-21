#include "includes.hpp"

#include <SFML/Window/Event.hpp>

#include "client_window.hpp"
#include "client.hpp"
#include "messenger.hpp"

// HACK!!!
#include "network/protocol.hpp"
#include "network/message.hpp"

namespace client {



ClientWindow::ClientWindow(ClientApp& app, const shared_ptr<Messenger>& messenger)
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
		mApp.stop();
	}

	// Process event loop
	sf::Event event;
	while (mRenderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mRenderWindow.close();
		if(mRenderer && (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased))
		{
			// HACK!!!
			proto::v1_0::to_srv::Control_KeyEvent ke;
			ke.type = (event.type == sf::Event::KeyPressed? proto::v1_0::to_srv::Control_KeyEvent::Evt_KeyDown : proto::v1_0::to_srv::Control_KeyEvent::Evt_KeyUp );
			ke.data.key = proto::v1_0::KeyMapper::Inst().toCode(event.key.code);
			ke.data.ch = 0;
			ke.data.modifier = 0;

			mApp.port()->send(net::make_message(ke));
		}
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