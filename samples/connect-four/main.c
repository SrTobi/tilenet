#include "4con.h"


TNSERVER GServer;
TNACCEPTOR GLocalAcceptor;


int move_sel(Layer* layer, int sel, int dt)
{
	int newsel = sel + dt;

	if(newsel < 0 || 3 <= newsel)
		return sel;

	print_chars(layer, 0, 2 + sel, 0, L"  ");
	print_chars(layer, 0, 2 + newsel, COLOR_LGREY, L"->");
	flush_layer(layer);

	return newsel;
}

int kick_to_ip(TNID player)
{
#define IP_BUF_SIZE 128
	wchar_t reason[IP_BUF_SIZE*2];
	wchar_t buf[IP_BUF_SIZE];
	FILE* file = fopen("ip.txt", "r");

	if(file && fgetws(buf, IP_BUF_SIZE, file) != NULL)
	{
		wcscpy(reason, L"@!!!link:");
		wcscat(reason, buf);
		tilenet_kick(player, reason);
		return 1;
	}

	return 0;
}

// Return 0 if we want to stop
// or the player id if the player wants to create a server
TNID doLocalMainMenu()
{
	TNKEYCODE key;
	TNKEYCODE upkey = 0, downkey = 0, enterkey = 0;
	TNEVENT evt;
	size_t fetched = 0;
	TNID player  = 0;
	int sel = 0;
	Layer* menuLayer = create_layer(20, 5);
	print_chars(menuLayer, 5, 0, COLOR_WHITE, L"Connect 4");
	print_chars(menuLayer, 0, 2, COLOR_LGREY, L"->Connect to ip");
	print_chars(menuLayer, 2, 3, COLOR_LGREY, L"Open server");
	print_chars(menuLayer, 2, 4, COLOR_LGREY, L"Exit");
	flush_layer(menuLayer);

	tilenet_keycode(L"up", &upkey);
	tilenet_keycode(L"down", &downkey);
	tilenet_keycode(L"enter", &enterkey);

	while(1)
	{
		tilenet_fetch_events(GServer, &evt, 1, &fetched, 0);

		switch(evt.type)
		{
		case TNEV_CONNECT:
			assert(!player);
			player = evt.participant;
			tilenet_attach_layer(player, menuLayer->handle);
			break;

		case TNEV_DISCONNECT:
			assert(player == evt.participant);
			return 0;

		case TNEV_KEYDOWN:
			assert(player == evt.participant);
			key = evt.data.keyevent.key;

			if(key == upkey || key == downkey)
			{
				sel = move_sel(menuLayer, sel, key == upkey? -1 : 1);
			}

			if(key == enterkey)
			{
				switch(sel)
				{
				case 0: // connect to ip
					if(kick_to_ip(player))
					{
						return 0;
					}else{
						// no ip.txt
					}
					break;
				case 1: // open server
					return player;
				case 2: // exit
					tilenet_destroy(GLocalAcceptor);
					break;
				default:
					assert(!"Case does not exist!");
				}
			}

		default:
			break;
		}
	}
}




int main(int argc, const char** args)
{
	TNSVRCONFIG config;
	int serverOnly = 0;
	TNID localPlayer;
	int idx;

	for(idx = 0; idx < argc; ++idx)
	{
		if(strcmp("--server", args[idx]) == 0)
		{
			serverOnly = 1;
		}
	}


	memset(&config, 0, sizeof(config));

	config.name = L"connect-4";
	config.info = L"Play 'connect-4' with friends over network!";
	config.pkg = L"connect-4-sample-package";
	config.pkgi = L"tn.org.samples.4con";
	config.options = 0;

	if(tilenet_create_server(&GServer, &config) != TNOK)
	{
		printf("Failed to create tilenet server!");
		return 1;
	}

	if(!serverOnly)
	{
		if(tilenet_add_local_acceptor(&GLocalAcceptor, GServer) != TNOK)
		{
			printf("Failed to create local player");
			return 2;
		}

		localPlayer = doLocalMainMenu();
	}

	if(localPlayer || serverOnly)
	{
		// start server
	}

	tilenet_exit(0);
	return 0;
}