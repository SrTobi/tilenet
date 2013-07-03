#ifndef _PLAYER_H
#define _PLAYER_H

#include "4con.h"

#define MAX_NAME_LEN 12
#define NAME_BUF_SIZE 16


typedef enum
{
	PlayerAttach,
	PlayerDetach,
	PlayerEvent,
} PlayerControl;


typedef void (*EvtHandler)(PlayerControl e, void* context, TNEVENT* evt);

typedef struct
{
	TNID id;
	void* context;
	EvtHandler handler;

	wchar_t name[NAME_BUF_SIZE];
} Player;




Player* get_player(TNID player);
void init_player_managment(unsigned int maxp);
void register_player(TNID id);
void unregister_player(TNID id);
void set_player_event_handler(TNID id, void* context, EvtHandler handler);
void notify_player_event(TNID id, TNEVENT* evt);




#endif