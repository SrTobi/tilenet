#include <iostream>
#include <cstring>
#include <string>
#include "tilenet.h"


#define EVENT_BUF_LENGTH 100
#define COL_WHITE 0xFFFFFFFF

#define LAYER_W	10
#define LAYER_H 10

TNSERVER srv;
TNID testTileId = 0;
TNID testSetId;
TNTILESET testSet;
TNLAYER testLayer;
TNACCEPTOR local_acceptor;
TNTILE* layerContent;


TNTILE& get_tile(unsigned int x, unsigned int y)
{
	return layerContent[y * LAYER_W + x];
}

void init_testlayer()
{
	tilenet_stdtile(L"test-a", &testTileId);

	tilenet_create_tilelayer(&testLayer, LAYER_W, LAYER_H, TNSTDRATIO, TNSTDRATIO, 0);
	layerContent = new TNTILE[LAYER_W * LAYER_H];
	memset(layerContent, 0, LAYER_W * LAYER_H * sizeof(TNTILE));

	TNTILE tile;
	tile.type = TN_STD_TILE;
	tile.data.stdset.id = testTileId;
	tile.data.stdset.color = COL_WHITE;
	tile.data.stdset.modifier = 0;

	get_tile(0, 0) = tile;

	tile.data.stdset.color = TNMAKE_COLOR(0xff, 0, 0xff, 0);
	get_tile(1, 0) = tile;

	tile.data.stdset.color = TNMAKE_COLOR(0xff, 0xff, 0, 0);
	get_tile(9, 9) = tile;
	tilenet_update_tilelayer(testLayer, layerContent, nullptr);
}

unsigned int pos = 0;

void update_layer()
{
	TNTILE tile;
	tile.type = TN_STD_TILE;
	tile.data.stdset.id = testTileId;
	tile.data.stdset.color = TNMAKE_COLOR(0xff, rand() % 0xff, rand() % 0xff, rand() % 0xff);
	tile.data.stdset.modifier = 0;

	layerContent[pos++ % (90) ] = tile;
	tilenet_update_tilelayer(testLayer, layerContent, nullptr);
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
	case TNEV_KEYDOWN:
		{
			TNKEYCODE key = e.data.keyevent.key;
			const wchar_t* name = L"";
			tilenet_keyname(key, &name);
			std::wcout << "Keydown[" << std::wstring(name) << "]\n";


			TNKEYCODE escape_code = 0;
			tilenet_keycode(L"esc", &escape_code);
			if(escape_code == key)
			{
				tilenet_kick(e.participant, L"");
				tilenet_exit(0);
				exit(0);
			}


		}break;
	default:
		break;
	}
}



int main()
{
	TNSVRCONFIG config;
	config.name = L"test";
	config.info = L"simple tilenet test";
	config.pkg = L"test-pack";
	config.pkgi = L"tn.org.samples.test";
	config.options = 0;

	init_testlayer();

	tilenet_set_service_thread_count(1);

	tilenet_create_server(&srv, &config);
	tilenet_add_local_acceptor(0, srv);


	while(true)
	{
		TNEVENT events[EVENT_BUF_LENGTH];

		size_t fetched;
		size_t timeout = 100;
		tilenet_fetch_events(srv, events, EVENT_BUF_LENGTH, &fetched, &timeout);

		if(fetched)
		{
			for(size_t i = 0; i < fetched; ++i)
			{
				do_event(events[i]);
			}
		}else{
			update_layer();
		}

	}




	std::cin.get();
	return 0;
}
