#include "includes.hpp"
#include "server/server.hpp"

namespace srv {


Server::Server(const TNSVRCONFIG* init)
{

}

Server::~Server()
{

}

size_t Server::destroy()
{
	NOT_IMPLEMENTED();
}

shared_ptr<TilenetObject> Server::clone()
{
	BOOST_THROW_EXCEPTION(excp::NotClonableException() << excp::InfoWhat(L"Server can not be cloned!"));
}











}