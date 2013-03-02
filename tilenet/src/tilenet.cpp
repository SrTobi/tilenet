#include "includes.h"
#include "tilenet.h"
#include "server/server.h"


namespace logg {
	std::wostream	_tnServerLogOutput(std::wclog.rdbuf());
	std::wostream	_tnClientLogOutput(std::wclog.rdbuf());
	std::wostream	_tnServiceLogOutput(std::wclog.rdbuf());
	uint			_tnCurrentDebugLevel = 0xFFFFFFFF;//Error | Info;
	boost::mutex	LogMutex;
}

namespace tn {
namespace helper {
	wstring ToWideString(const tn::string& str)
	{
		return sf::String(str);
	}

	string ToAnsiString(const tn::wstring& str)
	{
		return sf::String(str);
	}
}

TilenetConfig::TilenetConfig()
	:	serverName("[ServerName]"),
		serverInfo("[ServerInfo]"),
		tilePackageName("ascii"),
		tilePackageInterface("std"),
		maxConnections(40),
		standaloneServer(false),
		portBind(0),
		setAsActive(true)
{
}


Tilenet* Tilenet::Create(TilenetConfig& config)
{
	return new server::TilenetServer(config);
}
Tilenet* Tilenet::Create(string serverName, string serverInfo, string tilePackageName, uint   tilePackageVersion,
							string playerPassword, string observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
							bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
							unsigned short portBind, bool setAsActive, bool createStandartLayer)
{
	return nullptr;
}

Tilenet* Tilenet::Create(wstring serverName, wstring serverInfo, wstring tilePackageName, uint tilePackageVersion,
							wstring playerPassword, wstring observerPassword, uint waitForOnePlayerSec, uint maxPlayers,
							bool useAutoObservation, uint maxObserver, bool standaloneServer, bool useServerNetwork,
							unsigned short portBind, bool setAsActive, bool createStandartLayer)
{
	return nullptr;
}



/*
int Tilenet::_TilenetMain(	int argc, const char** argv,
								const wstring& servername, const wstring& serverinfo,
								uint maxObserver, const wstring& observerPassword,
								const wstring& tilepackname, uint tilepackversion,
								bool useLocalNet, void (*startMain)())
{
	return 0;
}


int Tilenet::_TilenetMain(	int argc, const char** argv,
								const string& servername, const string& serverinfo,
								uint maxObserver, const string& observerPassword,
								const string& tilepackname, uint tilepackversion,
								bool useLocalNet, void (*startMain)())
{
	//TODO Argumentline parsen

	TilenetConfig config;

	config.serverName = servername;
	config.serverInfo = serverinfo;

	config.tilePackageName = tilepackname;
	config.tilePackageVersion = tilepackversion;

	config.playerPassword = "";
	config.observerPassword = observerPassword;
	config.waitForOnePlayerSec = 3;

	config.maxPlayers = 1;
	config.useAutoObservation = true;
	config.maxObserver = maxObserver;

	config.standaloneServer = false;

	config.useServerNetwork = useLocalNet;
	// Suche zufaellig einen aus:
	config.portBind = 0;
	
	config.setAsActive = true;
	config.createStandartLayer = true;
//	try{
		Tilenet* tilenet = Tilenet::Create(config);
		tilenet->start();
		startMain();
	}catch(tn::TnException& e)
	{
		std::wcout << "Error: " << e.msg() << std::endl;
	}

	return 0;
}
*/

}