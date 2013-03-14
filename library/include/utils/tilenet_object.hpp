#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP

/// @cond DEV

#include <boost/noncopyable.hpp>
#include "settings.hpp"

#include "ptr.hpp"


/**
 * @brief Super class of most of tilenet's objects
 *
 * If an object is a subclass of Tilenetobject it can be used outside of tilenet with the c api.
 * It can also be used with ptr and weakptr
 *
 **/
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


/**
 * @brief Presents a weak ptr to a tilenet object or a subclass of it.
 *
 **/
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




/// @endcond

#endif