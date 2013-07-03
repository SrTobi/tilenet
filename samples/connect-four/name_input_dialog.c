#include "name_input_dialog.h"
#include "player.h"


#define ALLOWED_NAME_CHARS L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-_"

typedef struct
{
	Layer* layer;

	size_t len;
	wchar_t input[NAME_BUF_SIZE];
} NameInputDialog;



void handle_event(NameInputDialog* nip, TNEVENT* evt)
{
	TNBOOL redraw = 0;
	TNKEYCODE enterkey, backkey;
	tilenet_keycode(L"enter", &enterkey);
	tilenet_keycode(L"backspace", &backkey);

	if(evt->type == TNEV_KEYDOWN)
	{
		if(evt->data.keyevent.key == enterkey && nip->len >= 2)
		{
			// Done
			wcscpy(get_player(evt->participant)->name, nip->input);

			return;
		}else if(evt->data.keyevent.key == backkey && nip->len > 0)
		{
			--(nip->len);
			nip->input[nip->len] = L'\0';

			redraw = 1;
		}
	}

	if(evt->type == TNEV_TXT && nip->len < MAX_NAME_LEN)
	{
		wchar_t ch = evt->data.txtevent.ch;

		if(wcsrchr(ALLOWED_NAME_CHARS, ch))
		{
			nip->input[nip->len] = ch;
			nip->len++;
			nip->input[nip->len] = L'\0';
			redraw = 1;
		}

	}

	if(redraw)
	{
		print_chars(nip->layer, 0, 1, COLOR_LGREY, L"%-15s", nip->input);
		flush_layer(nip->layer);
	}
}


void nip_control(PlayerControl c, void* context, TNEVENT* evt)
{
	NameInputDialog* nip = (NameInputDialog*)context;

	switch(c)
	{
	case PlayerAttach:
		break;

	case PlayerDetach:
		destroy_layer(nip->layer);
		free(nip);
		break;

	case PlayerEvent:
		handle_event(nip, evt);
		break;

	default:
		assert(!"no default!");
	}
}



void attach_name_input_dialog( TNID player )
{
	NameInputDialog* nip = (NameInputDialog*)malloc(sizeof(NameInputDialog));

	nip->layer = create_layer(20, 2);
	nip->len = 0;
	nip->input[0] = L'\0';

	print_chars(nip->layer, 0, 0, COLOR_WHITE, L"Enter your name:");
	flush_layer(nip->layer);
	tilenet_attach_layer(player, nip->layer->handle);

	set_player_event_handler(player, nip, &nip_control);
}
