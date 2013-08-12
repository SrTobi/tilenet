#include <iostream>
#include <string>
#include <sstream>
#include <tiley/tiley.hpp>


#include "server.hpp"

void start_client(const std::string& str)
{
	tiley::ServerConfig<> config(L"multi-explore-redirect");
	config.description(L"Redirects the client to a certain ip!");
	config.package(L"multi-explore-sample-package");
	config.interface(L"tn.org.samples.multi-explore");

	tiley::Server<> server(config);

	tiley::LocalAcceptor acceptor(server);

	TNEVENT evt;
	while(server.fetch(evt))
	{
		if(evt.type == TNEV_CONNECT)
		{
			server.participant(evt.participant)->kick("@!!!link:" + str);
		}

		if(evt.type == TNEV_DISCONNECT)
		{
			break;
		}
	}

	tiley::WaitForClientToExit();
}






int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Usage:\n";
		std::cout << "multi-explore --server <port>\t[Start a server]\n";
		std::cout << "or\n";
		std::cout << "multi-explore <ip:port>\t[To connect to a server]\n";
		return 1;
	}

	if(argv[1] == std::string("--server"))
	{
		unsigned short port = 54321;
		// start server
		if(argc >= 3)
		{
			std::istringstream iss(argv[2]);
			iss >> port;
		}


		MEServer server(port);

		server.run();

	}else{
		// start client
		start_client(argv[1]);
	}

	return 0;
}