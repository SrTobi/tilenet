#include <iostream>
#include "tilenet.h"


#define EVENT_BUF_LENGTH 100


void do_event(TNEVENT& e)
{
	switch(e.type)
	{
	case TNEV_CONNECT:
		std::cout << "Player connected!\n";
		break;
	case TNEV_DISCONNECT:
		std::cout << "Player disconnected!\n";
		break;
	default:
		break;
	}
}



int main()
{
	TNSERVER srv;
	TNSVRCONFIG config;
	config.name = L"test";
	config.info = L"simple tilenet test";
	config.pkg = L"test-pkg";
	config.pkgi = L"test-interface";
	config.options = 0;

	tilenet_set_service_thread_count(1);

	tilenet_create_server(&srv, &config);
	tilenet_add_local_acceptor(srv);

	while(true)
	{
		TNEVENT events[EVENT_BUF_LENGTH];

		size_t fetched;
		tilenet_fetch_events(srv, events, EVENT_BUF_LENGTH, &fetched, 0);

		for(size_t i = 0; i < fetched; ++i)
		{
			do_event(events[i]);
		}

	}




	std::cin.get();
	return 0;
}
