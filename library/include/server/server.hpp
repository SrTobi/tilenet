#pragma once
#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <stack>
#include <vector>

#include "settings.hpp"


struct TilenetObject;

class IdManager
{
	friend struct TilenetObject;
public:
	typedef IdType id_type;
	IdManager();
	~IdManager();

	//RefObject* getObject(id_type id);

private:
	id_type reserveNewId();
	void	freeId(id_type id);

private:
	std::stack<id_type> mEmptyIds;
	std::vector<TilenetObject*> mObjects;
};

class Server
	: public IdManager
{
public:

private:

};



#endif