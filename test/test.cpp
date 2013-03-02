#include "../include/tiley.h"
#include <iostream>


#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


using namespace tny;
using namespace tn;



void sleep(int t)
{
#ifdef WIN32
	Sleep(t);
#else
	usleep(t*1000);
#endif
}







bool onePlayerHasConnected = false;
uint participantnumber;

Display* display = nullptr;
Layer* layer2 = nullptr;


class EHandler
	: public tn::Tilenet::EventHandler
{
public:
	EHandler(Tilenet* tnet)
		: server(tnet)
	{}

	uint eventBitmask()
	{
		return Bitmask::HandleNewParticipant | Bitmask::HandleParticipantDisconnect | Bitmask::HandleKeyEvent;
	}

	bool handleNewParticipant(uint p)
	{
		participantnumber = p;
		std::cout << "new participant!\n";
		Participant* par = server->acquireParticipant(p);

		par->attachDisplay(display);

		Participant::Release(par);
		onePlayerHasConnected = true;
		return true;
	}

	bool handleParticipantDisconnect(uint p)
	{
		std::cout << "Disconnect!\n";
		exit(0);
		return true;
	}

	void released()
	{
		delete this;
	}

	bool handleKeyEvent( uint p, const KeyEvent& e ) 
	{
		if(!e.isSpecialKey() || !e.isKeyPressed())
			return false;

		Point pos = layer2->position();
		Point dir;

		if(e.keyCode() == key::Escape) {
			exit(0);
		}else if(e.keyCode() == key::Right)
		{
			dir = Point(1, 0);
		}else if(e.keyCode() == key::Left)
		{
			dir = Point(-1, 0);
		}else if(e.keyCode() == key::Up)
		{
			dir = Point(0, -1);
		}else if(e.keyCode() == key::Down)
		{
			dir = Point(0, 1);
		}else{
			return false;
		}

		layer2->setPosition(pos + dir);

		return true;
	}

private:
	Tilenet* server;
};




int main()
{
	TilenetConfig config;

	config.serverName = "Test-Server";
	config.serverInfo = "Nur ein kleiner Test von tilenet";

	config.tilePackageName = "test";
	config.tilePackageInterface = "testint";

	config.maxConnections = 20;

	config.standaloneServer = true;

	config.useServerNetwork = false;
	// Suche zufaellig einen aus:
	config.portBind = 1234;
	config.setAsActive = true;

	Tilenet* tilenet = Tilenet::Create(config);

	display = tilenet->createDisplay();
	tilenet->registerEventHandler(new EHandler(tilenet));


	uint t = tilenet->tile(L"test");
	uint s = tilenet->tile(L"smiley");
	Layer* layer = display->createLayer(Point(20, 20), Rect(20, 20));
	layer2 = layer->createLayer(Point(30, 30), Rect(20, 20), tn::Layer::Below);


	for(int i = 0; i < 20; ++i)
		for(int j = 0; j < 20; ++j)
			layer->put(Point(i,j), t, 0xAA0000FF);

	for(int i = 0; i < 20; ++i)
		for(int j = 0; j < 20; ++j)
			layer2->put(Point(i,j), t, 0xFF00FF00);


	tilenet->start();
	while(!onePlayerHasConnected)
		tilenet->yield();



	//Layer* fg =  layer->createLayer(Point(20,20), Rect(20,20), tn::Layer::Below);  
	//GetPlayerDisplay()->createLayer(Point(0,0), Rect(20, 20));

	///Layer* layer2 = layer->createLayer(Point(0,20), Rect(20, 20), tn::Layer::Above);
	/*for(int i = 0; i < 20; ++i)
		for(int j = 0; j < 20; ++j)
			layer->put(Point(i,j), s, 0xFF00FF00);

	  for(int i = 0; i < 20; ++i)
		for(int j = 0; j < 20; ++j)
			fg->put(Point(i,j), t, 0xFFFFFF00);*/

	
	/*for(int x = 0; x < 1000; ++x)
	{
		for(int i = 0; i < 20; ++i)
			for(int j = 0; j < 20; ++j)
				layer->put(Point(i,j), s, 0xFF000000 | rand());
		sleep(50);
		}*/

	while(true)
	{

		tilenet->yield();
		sleep(50);
	/*  for(int i = 0; i < 20; ++i)
			for(int j = 0; j < 20; ++j)
				layer->put(Point(i,j), s, 0xFF00FF00 | rand() & 1);

		  for(int i = 0; i < 20; ++i)
			for(int j = 0; j < 20; ++j)
				fg->put(Point(i,j), t, 0xFFF0F000 | rand() & 1);
		  //	fg->put(Point(rand() % 5, rand() % 5), t, 0xFF505050 | rand() << 16 );
	*/}
	
}
