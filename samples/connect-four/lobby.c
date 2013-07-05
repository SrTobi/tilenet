#include "lobby.h"
#include "player.h"

TNID gameslots[10];
Layer* lobby_layer = NULL;


void free_slot(int slot)
{
	print_chars(lobby_layer, 3, 3 + slot, COLOR_LGREY, L"<empty>                                 ");
	gameslots[slot] = 0;
}

void free_player_slot(TNID p)
{
	int idx;

	for(idx = 0; idx <= 9; ++idx)
	{
		if(gameslots[idx] == p)
			free_slot(idx);
	}
}

void process_slot(int slot, TNID p)
{
	if(gameslots[slot])
	{
		if(gameslots[slot] == p)
			return;

		// found two players... create a game
		free_slot(slot);
	}else{
		free_player_slot(p);

		// open new slot
		print_chars(lobby_layer, 3, 3 + slot, COLOR_LGREY, L"%s is waiting                    ", get_player(p)->name);
		gameslots[slot] = p;
	}
	flush_layer(lobby_layer);
}

void handle_lobby_event(TNEVENT* evt)
{
	wchar_t ch;
	int idx;
	int slot;
	int count;
	TNKEYCODE esckey;

	if(evt->type == TNEV_TXT)
	{
		ch = evt->data.txtevent.ch;

		if(L'0' <= ch && ch <= '9' )
		{
			slot = ch - '0';
			assert(0 <= slot && slot <= 9);

			process_slot(slot, evt->participant);

		} else if (ch == L'r')
		{
			int slot = rand() % 10;

			for(idx = 0, count = 0; count < slot || gameslots[idx % 10] == 0; idx++)
			{
				if(idx >= 10 && count == 0)
				{
					// No games... open a new one
					process_slot(0, evt->participant);
					break;
				}

				if(gameslots[idx % 10])
					++count;
			}


		}

	}else if(evt->type == TNEV_KEYDOWN)
	{
		tilenet_keycode(L"escape", &esckey);

		if(evt->data.keyevent.key == esckey)
		{
			free_player_slot(evt->participant);
			flush_layer(lobby_layer);
		}

	}
}




void lobby_control(PlayerControl c, void* context, TNEVENT* evt)
{
	switch(c)
	{
	case PlayerAttach:
		break;

	case PlayerDetach:
		free_player_slot(evt->participant);
		flush_layer(lobby_layer);
		break;

	case PlayerEvent:
		handle_lobby_event(evt);
		break;

	default:
		assert(!"no default!");
	}
}




void add_player_to_lobby( TNID player )
{
	int i = 0;

	if(!lobby_layer)
	{
		// init lobby
		memset(gameslots, 0, sizeof(gameslots));

		lobby_layer = create_layer(35, 15);

		print_chars(lobby_layer, 0,0, COLOR_WHITE, L"Select a game or open a new one\nby pressing 0-9.");

		for(i = 0; i < 10; ++i)
			print_chars(lobby_layer, 0, 3 + i, COLOR_LGREY, L"%i: <empty>", i);

		print_chars(lobby_layer, 0, 14, 0xff00bb00, L"0-9: select game   r: random");
		flush_layer(lobby_layer);
	}

	tilenet_attach_layer(player, lobby_layer->handle);
	set_player_event_handler(player, NULL, &lobby_control);
}
