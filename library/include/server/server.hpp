#pragma once
#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <stack>
#include <vector>

#include "settings.hpp"
#include "utils/tilenet_object.hpp"

namespace srv {


class Server
	: public TilenetObject
{
public:
	Server(const TNSVRCONFIG* init);
	~Server();

	virtual override size_t destroy();
	virtual override shared_ptr<TilenetObject> clone();

private:
};



}



#endif