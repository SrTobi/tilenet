#include "includes.hpp"
#include "client_window.hpp"


namespace client {



ClientWindow::ClientWindow()
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

}



}