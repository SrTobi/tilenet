#include "4con.h"
#include "player.h"
#include "name_input_dialog.h"

TNSERVER GServer;
TNACCEPTOR GLocalAcceptor;
TNACCEPTOR GListenAcceptor;



int read_from_file(const char* filename, wchar_t* buf, size_t buflen)
{
#pragma warning(disable: 4996)
	FILE* file = fopen(filename, "r");

	if(file && fgetws(buf, buflen, file) != NULL)
	{
		return 1;
	}
	return 0;
}


////////////////////////////////////////////////////////////////// Server //////////////////////////////////////////////////////////



int start_server()
{
#define PORT_BUF_SIZE 8
#define MAX_PLAYER_BUF 8
	wchar_t portBuf[PORT_BUF_SIZE];
	wchar_t maxPBuf[MAX_PLAYER_BUF];

	unsigned short port = 0;
	unsigned int maxPlayer = 0;

	if(!read_from_file("port.txt", portBuf, PORT_BUF_SIZE))
	{
		return 0;
	}

	if(read_from_file("max-player.txt", portBuf, PORT_BUF_SIZE))
	{
		maxPlayer = wcstoul(maxPBuf, 0, 10);
	}

	port = (unsigned short)wcstoul(portBuf, 0, 10);
	if(!port)
		return 0;

	if(!maxPlayer)
		maxPlayer = 15;

	if(tilenet_add_listen_acceptor(&GListenAcceptor, GServer, port, maxPlayer) != TNOK)
	{
		return 0;
	}

	init_player_managment(maxPlayer);

	return 1;
}

void add_new_player(TNID p)
{
	register_player(p);
	attach_name_input_dialog(p);
}


void run_server(TNID localPlayer)
{
#define EVENT_BUF_SIZE 32
	size_t fetched = 0;
	TNEVENT evt;

	if(localPlayer)
		add_new_player(localPlayer);

	while(1)
	{
		tilenet_fetch_events(GServer, &evt, 1, &fetched, 0);
		assert(fetched == 1);

		switch (evt.type)
		{
		case TNEV_CONNECT:
			add_new_player(evt.participant);
			break;

		case TNEV_DISCONNECT:
			unregister_player(evt.participant);
			break;

		default:
			notify_player_event(evt.participant, &evt);
		}
	}
}








////////////////////////////////////////////////////////////////// Menu //////////////////////////////////////////////////////////

#define MENU_CONNECT_TO_IP 0
#define MENU_CREATE_SERVER 1
#define MENU_EXIT 2

#define MENU_MIN 0
#define MENU_MAX 2

#define MENU_HEAD_COLOR		COLOR_WHITE
#define MENU_COLOR			COLOR_LGREY
#define MENU_WARN_COLOR		COLOR_RED

int kick_to_ip(TNID player)
{
#define IP_BUF_SIZE 128
	wchar_t reason[IP_BUF_SIZE*2];
	wchar_t buf[IP_BUF_SIZE];

	if(read_from_file("ip.txt", buf, IP_BUF_SIZE))
	{
		wcscpy(reason, L"@!!!link:");
		wcscat(reason, buf);
		tilenet_kick(player, reason);
	}
	return 0;
}

void displayInfo(Layer* layer, const wchar_t* info, TNCOLOR color)
{
	print_chars(layer, 0, 6, color, L"%-20ls", info);
}

void print_sel_info(Layer* layer, int sel)
{
	switch (sel)
	{
	case MENU_CONNECT_TO_IP:
		displayInfo(layer, L"Reads ip from ip.txt", MENU_COLOR);
		break;

	case MENU_CREATE_SERVER:
		displayInfo(layer, L"Open a tcp server", MENU_COLOR);
		break;

	case MENU_EXIT:
		displayInfo(layer, L"Exit to OS", MENU_COLOR);
		break;

	default:
		break;
	}
}

int move_sel(Layer* layer, int sel, int dt)
{
	int newsel = sel + dt;

	if(newsel < MENU_MIN || MENU_MAX < newsel)
		return sel;

	print_chars(layer, 0, 2 + sel, MENU_COLOR, L"  ");
	print_chars(layer, 0, 2 + newsel, MENU_COLOR, L"->");
	print_sel_info(layer, newsel);
	flush_layer(layer);

	return newsel;
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
	Layer* menuLayer = create_layer(20, 7);
	print_chars(menuLayer, 5, 0, MENU_HEAD_COLOR, L"Connect 4");
	print_chars(menuLayer, 2, 2, MENU_COLOR, L"Connect to ip");
	print_chars(menuLayer, 2, 3, MENU_COLOR, L"Open server");
	print_chars(menuLayer, 2, 4, MENU_COLOR, L"Exit");
	move_sel(menuLayer, sel , 0);

	tilenet_keycode(L"up", &upkey);
	tilenet_keycode(L"down", &downkey);
	tilenet_keycode(L"enter", &enterkey);

	while(1)
	{
		tilenet_fetch_events(GServer, &evt, 1, &fetched, 0);

		assert(fetched == 1);

		switch(evt.type)
		{
		case TNEV_CONNECT:
			assert(!player);
			player = evt.participant;
			tilenet_attach_layer(player, menuLayer->handle);
			break;

		case TNEV_DISCONNECT:
			assert(player == evt.participant);
			destroy_layer(menuLayer);
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
				case MENU_CONNECT_TO_IP: // connect to ip
					if(!kick_to_ip(player))
					{
						//print_chars(menuLayer, )
						displayInfo(menuLayer, L"Failed to load ip.txt", MENU_WARN_COLOR);
						flush_layer(menuLayer);
					}
					break;
				case MENU_CREATE_SERVER: // open server

					if(start_server())
					{
						destroy_layer(menuLayer);
						return player;
					}else{
						displayInfo(menuLayer, L"Failed to start server!", MENU_WARN_COLOR);
						flush_layer(menuLayer);
					}
					break;
				case MENU_EXIT: // exit
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
	TNID localPlayer = 0;
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
		run_server(localPlayer);
	}

	tilenet_exit(0);
	return 0;
}