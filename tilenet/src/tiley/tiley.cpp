#include "includes.h"
#include "tiley.h"
#include "server/server.h"

namespace tny {



tn::Tilenet* GetTilenetServer()
{
	return server::TilenetServer::Inst();
}


tn::Display* GetPlayerDisplay()
{
	NOT_IMPL();
	//return server::TilenetServer::Inst()->getMainPlayer();
}



}
