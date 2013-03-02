

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "tilenet.h"
#include "client/client.h"

using namespace std;




int main(int argc, char** argv)
{
	if(argc < 3)
	{
		cout << "Nicht genug Arguente!\n";

		return 1;
	}


	boost::shared_ptr<client::ClientApp> app = boost::make_shared<client::ClientApp>(sf::String(argv[1]), atoi(argv[2]));

	app->run();
}