#pragma once
#ifndef _TN_TILEY_H
#define _TN_TILEY_H


#include "tilenet.h"
//#include "tilex.h"

namespace tny 
{


tn::Tilenet* GetTilenetServer();
tn::Display* GetPlayerDisplay();


}


#ifndef TN_NO_EASY_MAIN
#define TileNetMain(_servername, _serverinfo, _tilepackname)	\
					TileNetMainEx(_servername, _serverinfo, 3, "", _tilepackname, 1, false)

#define TileNetMainEx(_servername, _serverinfo, _maxObserver,	\
						_observerPassword, _tilepackname,		\
							_tilepackversion, _useLocalNet)												\
		void tnMain();																					\
		int main(int argc, const char** argv)															\
		{	return tn::Tilenet::_TilenetMain(argc, argv,												\
								_servername, _serverinfo, _maxObserver,									\
								_observerPassword, _tilepackname, _tilepackversion, _useLocalNet, &tnMain);}	void tnMain
#endif





#endif