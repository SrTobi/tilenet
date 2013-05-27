#include <iostream>
#include "tilenet.h"


#define EVENT_BUF_LENGTH 100
#define COL_WHITE 0xFFFFFFFF

TNID testTileId = 0;
TNID testSetId;
TNTILESET testSet;
TNLAYER testLayer;


void init_testlayer()
{
	tilenet_create_tileset(&testSet, &testSetId, L"#debug-std-tileset", 0);
	tilenet_register_tile(testSet, L"test-a", &testTileId);

	tilenet_create_tilelayer(&testLayer, 10, 10, TNSTDRATIO, TNSTDRATIO, 0);

	TNTILE tile;
	tile.tileset = testSetId;
	tile.data.stdset.id = testTileId;
	tile.data.stdset.color = COL_WHITE;
	tile.data.stdset.modifier = 0;

	tilenet_put_tile(testLayer, 0, 0, &tile);

	tile.data.stdset.color = TNMAKE_COLOR(0xff, 0, 0xff, 0);
	tilenet_put_tile(testLayer, 1, 0, &tile);

	tile.data.stdset.color = TNMAKE_COLOR(0xff, 0xff, 0, 0);
	tilenet_put_tile(testLayer, 9, 9, &tile);
}

void do_event(TNEVENT& e)
{
	switch(e.type)
	{
	case TNEV_CONNECT:
		std::cout << "Player connected!\n";

		tilenet_attach_layer(e.participant, testLayer);
		break;
	case TNEV_DISCONNECT:
		std::cout << "Player disconnected!\n";
		break;
	default:
		break;
	}
}



int main()
{
	TNSERVER srv;
	TNSVRCONFIG config;
	config.name = L"test";
	config.info = L"simple tilenet test";
	config.pkg = L"test-pkg";
	config.pkgi = L"test-interface";
	config.options = 0;

	init_testlayer();

	tilenet_set_service_thread_count(1);

	tilenet_create_server(&srv, &config);
	tilenet_add_local_acceptor(srv);


	while(true)
	{
		TNEVENT events[EVENT_BUF_LENGTH];

		size_t fetched;
		tilenet_fetch_events(srv, events, EVENT_BUF_LENGTH, &fetched, 0);

		for(size_t i = 0; i < fetched; ++i)
		{
			do_event(events[i]);
		}

	}




	std::cin.get();
	return 0;
}
