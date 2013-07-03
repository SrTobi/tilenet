#include "player.h"



typedef struct
{
	unsigned int maxp;
	Player* players;
} PlayerManager;

PlayerManager GPlM;

Player* get_player(TNID id)
{
	TNID idx = TNEXTRACTID(id);
	assert(idx);
	assert(idx <= GPlM.maxp);
	assert(GPlM.players[idx].id == id);

	return GPlM.players + idx;
}



void init_player_managment( unsigned int maxp )
{
	GPlM.maxp = maxp;
	GPlM.players = (Player*)malloc(sizeof(Player) * (maxp + 1));
	memset(GPlM.players, 0, sizeof(Player) * maxp);
}


void register_player( TNID id )
{
	TNID idx = TNEXTRACTID(id);

	assert(idx);
	assert(idx <= GPlM.maxp);
	assert(GPlM.players[idx].id == 0);

	GPlM.players[idx].id = id;
}

void unregister_player( TNID id )
{
	Player* p = get_player(id);

	assert(p->handler);
	p->handler(PlayerDetach, p->context, 0);
	p->id = 0;
	p->handler = 0;
	p->context = 0;
}

void set_player_event_handler( TNID id, void* context, EvtHandler handler )
{
	Player* p = get_player(id);

	if(p->handler)
		p->handler(PlayerDetach, p->context, 0);

	p->handler = handler;
	p->context = context;
	p->handler(PlayerAttach, p->context, 0);
}

void notify_player_event(TNID id, TNEVENT* evt )
{
	Player* p = get_player(id);

	assert(p->handler);
	p->handler(PlayerEvent, p->context, evt);

}

