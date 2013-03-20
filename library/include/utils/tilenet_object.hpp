#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP

/// @cond DEV

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include "settings.hpp"


/**
 * @brief Super class of most of tilenet's objects
 *
 * If an object is a subclass of Tilenetobject it can be used outside of tilenet with the c api.
 *
 **/
struct TilenetObject
{
	template<typename Type, typename TnObject>
	friend class ptr;
public:
	TilenetObject();
	virtual ~TilenetObject();

	virtual size_t destroy() = 0;
	virtual shared_ptr<TilenetObject> clone() = 0;
};


template<typename T>
class IdObject
{
	typedef TNID							id_type;
	typedef std::vector<T>					list_type;
	typedef std::queue<id_type>				queue_type;
public:
	IdObject();
	virtual ~IdObject();

	virtual override size_t destroy() = 0;
	virtual override shared_ptr<TilenetObject> clone() = 0;

	id_type id() const;

	static id_type Register(const shared_ptr<T>& obj);
	static shared_ptr<T> Resolve(id_type id);
private:
	id_type	mId;
	static list_type			Objects;
	static queue_type			FreeList;
	static boost::shared_mutex	IdMutex;
};


template<typename T>
IdObject<T>::IdObject()
{
}

template<typename T>
IdObject<T>::~IdObject()
{
}

template<typename T>
typename IdObject<T>::id_type IdObject<T>::id() const
{
	return mId;
}

template<typename T>
typename IdObject<T>::id_type IdObject<T>::Register(const shared_ptr<T>& obj)
{
	boost::lock_guard<boost::shared_mutex> lock(IdMutex);

	id_type newId;
	if(FreeList.size())
	{
		newId = FreeList.front();
		FreeList.pop();

		assert(newId < Objects.size());
		assert(!Objects.at(newId));
		Objects[newId] = obj;
	}else{
		newId = Objects.size();
		Objects.push_back(obj);
	}

	// TODO: Add random tag
	
	obj->id = newId;

	return newId;
}

template<typename T>
shared_ptr<T> IdObject<T>::Resolve(id_type id)
{
	boost::shared_lock<boost::shared_mutex> lock(IdMutex);

	id_type rawId = TNEXTRACTID_ID(id);

	if(rawId < Objects.size())
	{
		shared_ptr<T> obj = Objects[rawId].lock();

		if(obj && obj->id() == id)
			return obj;
	}
	
	// TODO: throw!
}


/// @endcond

#endif