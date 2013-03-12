#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP


#include "settings.hpp"

#include "ptr.hpp"

template<typename Type>
class ptr;

class Server;
class TilenetWeakObject;

struct TilenetObject
{
	template<typename Type>
	friend class ptr;
public:
	TilenetObject();
	virtual ~TilenetObject();


	size_t refCount() const;
	size_t weakCount() const;

	virtual ptr<TilenetWeakObject> weak();

private:
	void addref() const;
	size_t subref() const;

private:
	mutable size_t			mCount;
	ptr<TilenetWeakObject>	mWeak;
};



class TilenetWeakObject
	: public TilenetObject
{
	friend struct TilenetObject;
public:
	virtual ~TilenetWeakObject();

	virtual override ptr<TilenetWeakObject> weak();

	ptr<TilenetObject> unweak();
	ptr<const TilenetObject> unweak() const;

private:
	TilenetWeakObject(const ptr<TilenetObject>& obj);

	TilenetObject*const mObj;
};




class TilenetIdObject
	: public TilenetObject
{
public:
	TilenetIdObject();
	virtual ~TilenetIdObject();
};


#endif