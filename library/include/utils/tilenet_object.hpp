#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP

#include <boost/noncopyable.hpp>
#include "settings.hpp"

#include "ptr.hpp"


struct TilenetObject
{
	template<typename Type, typename TnObject>
	friend class ptr;
public:
	TilenetObject();
	virtual ~TilenetObject();


	size_t refCount() const;
	size_t weakCount() const;

	virtual ptr<TilenetWeakObject> weak();
	virtual size_t destroy();
private:
	void addref() const;
	bool subref() const;

	static void Release(const TilenetObject* obj);
private:
	mutable size_t			mCount;
	ptr<TilenetWeakObject>	mWeak;
};



class TilenetWeakObject
	: public TilenetObject
	, boost::noncopyable
{
	friend struct TilenetObject;
	template<typename Type, typename TnWeakObject, typename TnObject>
	friend class weakptr;
public:
	virtual ~TilenetWeakObject();

	virtual override ptr<TilenetWeakObject> weak();
	virtual override size_t destroy();

	ptr<TilenetObject> unweak();
	ptr<const TilenetObject> unweak() const;
private:
	void uncouple();
	TilenetWeakObject(TilenetObject* obj);

	TilenetObject* mObj;
};




class TilenetIdObject
	: public TilenetObject
{
public:
	TilenetIdObject();
	virtual ~TilenetIdObject();
};


#endif