#include "game.h"
#include "player.h"

#define BOARD_W 7
#define BOARD_H 6


TNID token_tile =  0;
TNID side_tile = 0;
TNID bottom_tile = 0;
TNID lcorner_tile = 0;
TNID rcorner_tile = 0;
TNID cursor_tile = 0;

void init_tiles()
{
	if(cursor_tile == 0)
	{
		token_tile = get_tileid(L"board-token");
		side_tile = get_tileid(L"board-side");
		bottom_tile = get_tileid(L"board-bottom");
		lcorner_tile = get_tileid(L"board-left-corner");
		rcorner_tile = get_tileid(L"board-right-corner");
		cursor_tile = get_tileid(L"board-cursor");
	}
}



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
	TNID board[BOARD_W][BOARD_H];
} Game;

typedef struct _GameView
{
	struct _Game* game;
	TNID pid;

	struct _GameView* other;

	int cursor;
	Layer* board_layer;
} GameView;



void move_cursor(GameView* gv, int dt)
{
	int new_cursor = gv->cursor;
	if(new_cursor < 0 || new_cursor >= BOARD_W)
		return;

	set_nulltile(gv->board_layer, 1 + new_cursor, 0);
	set_stdtile(gv->board_layer, 1 + new_cursor, 0, cursor_tile, COLOR_BLUE);
	flush_layer(gv->board_layer);
}

void handle_game_event(GameView* g, TNEVENT* evt)
{

}

void game_control(PlayerControl c, void* context, TNID p, TNEVENT* evt)
{
	GameView* gv = (GameView*)context;


	switch(c)
	{
	case PlayerAttach:
		break;

	case PlayerDetach:
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
	Layer* layer = create_layer(BOARD_W + 2, BOARD_H + 2);
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
	int x,y;
	Game* g = (Game*)malloc(sizeof(Game));

	// clear
	for(x = 0; x < BOARD_W; ++x)
		for(y = 0; y < BOARD_H; ++y)
			g->board[x][y] = 0;

	init_tiles();

	gv1 = create_game_view(p1, g);
	gv2 = create_game_view(p2, g);

	gv1->other = gv2;
	gv2->other = gv1;

	set_player_event_handler(p1, gv1, &game_control);
	set_player_event_handler(p2, gv2, &game_control);
}