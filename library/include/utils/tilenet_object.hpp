#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP


#include "settings.hpp"

template<typename Type>
class ptr;

class Server;
struct TilenetWeakObject;

struct TilenetObject
{
public:
	TilenetObject();
	~TilenetObject();


	size_t refCount() const;
	size_t weakCount() const;

	virtual Server* server() const;

protected:
	virtual TilenetWeakObject* weak() const;

private:
	void addref();
	void subref();

private:
	size_t		mCount;
};

struct TilenetWeakObject
	: public TilenetObject
{
	template<typename Type>
	friend class ptr;
	template<typename Type>
	friend class weakptr;
public:
	TilenetWeakObject(TilenetObject* obj);
	~TilenetWeakObject();

	virtual override Server* server() const;

private:
	TilenetObject* unweak() const;
	virtual override TilenetWeakObject* weak() const;
private:
	TilenetObject* mObj;
};




#endif