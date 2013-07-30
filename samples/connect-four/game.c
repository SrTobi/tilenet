#include "game.h"
#include "player.h"
#include "lobby.h"

#include <stdarg.h>

#define BOARD_W 7
#define BOARD_H 6


TNID token_tile =  0;
TNID side_tile = 0;
TNID bottom_tile = 0;
TNID lcorner_tile = 0;
TNID rcorner_tile = 0;
TNID cursor_tile = 0;

TNKEYCODE escape_key = 0;
TNKEYCODE use_key = 0;
TNKEYCODE left_key = 0;
TNKEYCODE right_key = 0;
TNKEYCODE enter_key = 0;

void init_tiles()
{
	if(cursor_tile == 0)
	{
		token_tile = get_tileid(L"board::token");
		side_tile = get_tileid(L"board::side");
		bottom_tile = get_tileid(L"board::bottom");
		lcorner_tile = get_tileid(L"board::left-corner");
		rcorner_tile = get_tileid(L"board::right-corner");
		cursor_tile = get_tileid(L"board::cursor");

		tilenet_keycode(L"escape", &escape_key);
		tilenet_keycode(L"space", &use_key);
		tilenet_keycode(L"left", &left_key);
		tilenet_keycode(L"right", &right_key);
		tilenet_keycode(L"enter", &enter_key);
	}
}


struct _GameView;
/*
 *	Board:
 *      v
 *	|		|
 *	|		|
 *	|		|
 *	|		|
 *	|	X	|
 *	|   XX  |
 *	*-------*
 */
typedef struct _Game
{
	TNID board[BOARD_W+2][BOARD_H+2];
	TNID turn;

	struct _GameView* p1;
	struct _GameView* p2;
} Game;

typedef struct _GameView
{
	struct _Game* game;
	TNID pid;

	struct _GameView* other;

	int cursor;
	Layer* board_layer;
} GameView;



void set_status_line(Layer* layer, TNCOLOR color, const wchar_t* txt, ...)
{
#define BUFFER_SIZE 256
	wchar_t buf[BUFFER_SIZE];
	int len;

	va_list args;
	va_start(args, txt);

	len = vswprintf(buf, BUFFER_SIZE, txt, args);

	wcscat(buf, L"                ");
	print_chars(layer, 0, BOARD_H + 2, color, buf);

	va_end(args);

	flush_layer(layer);
}


void print_player_turn(GameView* gv, TNID turn)
{
	if(turn == gv->pid)
	{
		set_status_line(gv->board_layer, COLOR_LGREY, L"Your turn");
	}else{
		set_status_line(gv->board_layer, COLOR_LGREY, L"%lss turn", get_player(turn)->name);
	}
}

void print_turn(Game* g)
{
	print_player_turn(g->p1, g->turn);
	print_player_turn(g->p2, g->turn);
}

void move_cursor(GameView* gv, int dt)
{
	int new_cursor = gv->cursor + dt;
	if(new_cursor < 0 || new_cursor >= BOARD_W)
		return;

	set_nulltile(gv->board_layer, 1 + gv->cursor, 0);
	set_stdtile(gv->board_layer, 1 + new_cursor, 0, cursor_tile, COLOR_BLUE);
	flush_layer(gv->board_layer);

	gv->cursor = new_cursor;
}

void start_game(Game* g)
{
	int x,y;

	// clear board
	for(x = 0; x < BOARD_W + 2; ++x)
		for(y = 0; y < BOARD_H + 2; ++y)
		{
			g->board[x][y] = 0;
			if(x < BOARD_W && y < BOARD_H)
			{
				set_nulltile(g->p1->board_layer, 1 + x, 1 + y);
				set_nulltile(g->p2->board_layer, 1 + x, 1 + y);
			}
		}

	// clear layer


	g->turn = (rand() % 2? g->p1->pid : g->p2->pid);

	print_turn(g);
}



void mark_win(Game* g, int x, int y, int dx, int dy, TNID turn)
{
	while(g->board[x][y] == turn)
	{
		x += dx; y += dy;
	}

	x -= dx; y -= dy;

	do {
		set_stdtile(g->p1->board_layer, x, y, token_tile, 0xFFFFFF00);
		set_stdtile(g->p2->board_layer, x, y, token_tile, 0xFFFFFF00);
		x -= dx; y -= dy;

	}while(g->board[x][y] == turn);

}

TNBOOL check_dir(Game* g, int x, int y, int dx, int dy, TNID turn)
{
	int count = 0;
	while(g->board[x][y] == turn)
	{
		x += dx; y += dy;
	}

	x -= dx; y -= dy;

	while(g->board[x][y] == turn)
	{
		++count;
		x -= dx; y -= dy;
	}

	return count >= 4? 1 : 0;
}

void print_player_win(GameView* gv, TNID turn)
{
	if(gv->pid == turn)
	{
		set_status_line(gv->board_layer, COLOR_GREEN, L"Your win!");
	}else{
		set_status_line(gv->board_layer, COLOR_RED, L"You lose!");
	}
}

void print_win(Game* g, TNID turn)
{
	print_player_win(g->p1, turn);
	print_player_win(g->p2, turn);
}

TNBOOL check_win(Game* g, int x, int y, TNID turn)
{
	int dirs[4][2] = {	1, 1,
						1, 0,
						0, 1,
						1, -1};
	int idx;
	for(idx = 0; idx < 4; ++idx)
	{
		if(check_dir(g, x, y, dirs[idx][0], dirs[idx][1], turn))
		{
			mark_win(g, x, y, dirs[idx][0], dirs[idx][1], turn);
			print_win(g, turn);
			return 1;
		}
	}
	return 0;
}

void do_turn(GameView* gv)
{
	int idx;
	Game* g = gv->game;
	TNID* column = g->board[gv->cursor+1];
	GameView* other = gv->other;
	TNID turn = g->turn;
	TNID pid = gv->pid;

	// check if fulls and if it is our turn
	if(pid != turn || column[1] != 0)
		return;

	for(idx = 1; idx <= BOARD_H && column[idx] == 0; ++idx);

	--idx;
	// update
	column[idx] = pid;
	set_stdtile(gv->board_layer, gv->cursor + 1, idx, token_tile, COLOR_GREEN);
	set_stdtile(other->board_layer, gv->cursor + 1, idx, token_tile, COLOR_RED);
	
	if(check_win(g, gv->cursor + 1, idx, turn))
	{
		g->turn = 0;
	}else{
		g->turn = other->pid;
		print_turn(g);
	}

}

void handle_game_event(GameView* gv, TNEVENT* evt)
{
	if(evt->type == TNEV_KEYDOWN)
	{
		TNKEYCODE key = evt->data.keyevent.key;

		if(key == escape_key)
		{
			add_player_to_lobby(evt->participant);
		}else if (key == left_key || key == right_key)
		{
			move_cursor(gv, key == left_key? -1 : 1);
		}else if(key == use_key)
		{
			do_turn(gv);
		}else if(key == enter_key && gv->game->turn == 0)
		{
			start_game(gv->game);
		}
	}
}


void game_control(PlayerControl c, void* context, TNID p, TNEVENT* evt)
{
	GameView* gv = (GameView*)context;

	switch(c)
	{
	case PlayerAttach:
		break;

	case PlayerDetach:
		if(gv->game)
		{
			free(gv->game);
		}

		if(gv->other)
		{
			assert(gv->other->game != NULL);
			gv->other->other = NULL;
			gv->other->game = NULL;

			add_player_to_lobby(gv->other->pid);
		}

		destroy_layer(gv->board_layer);
		free(gv);
		break;

	case PlayerEvent:
		handle_game_event(gv, evt);
		break;

	default:
		assert(!"no default!");
	}
}


GameView* create_game_view(TNID pid, Game* g)
{
	int x,y;
	Layer* layer = create_layer(BOARD_W + 10, BOARD_H + 3, L"game");
	GameView* gv = (GameView*)malloc(sizeof(GameView));

	gv->pid = pid;
	gv->game = g;
	gv->cursor = BOARD_W / 2;
	gv->board_layer = layer;

	//////// create layer ////////
	// side
	for(y = 1; y <= BOARD_H; ++y)
	{
		set_stdtile(layer, 0, y, side_tile, COLOR_LGREY);
		set_stdtile(layer, BOARD_W + 1, y, side_tile, COLOR_LGREY);
	}

	set_stdtile(layer, 0, y, lcorner_tile, COLOR_LGREY);

	// bottom
	for(x = 1; x <= BOARD_W; ++x)
	{
		set_stdtile(layer, x, y, bottom_tile, COLOR_LGREY);
	}

	set_stdtile(layer, x, y, rcorner_tile, COLOR_LGREY);

	move_cursor(gv, 0);
	tilenet_attach_layer(pid, layer->handle);

	return gv;
}

void create_game( TNID p1, TNID p2 )
{
	GameView* gv1;
	GameView* gv2;
	Game* g = (Game*)malloc(sizeof(Game));

	init_tiles();

	g->p1 = gv1 = create_game_view(p1, g);
	g->p2 = gv2 = create_game_view(p2, g);

	gv1->other = gv2;
	gv2->other = gv1;

	start_game(g);

	set_player_event_handler(p1, gv1, &game_control);
	set_player_event_handler(p2, gv2, &game_control);
}