#pragma once
#ifndef _TILENET_OBJECT_HPP
#define _TILENET_OBJECT_HPP

/// @cond DEV

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <mutex>

#include "settings.hpp"


/**
 * @brief Super class of most of tilenet's objects
 *
 * If an object is a subclass of Tilenetobject it can be used outside of tilenet with the c api.
 *
 **/
struct TilenetObject
	: public std::enable_shared_from_this<TilenetObject>
{
public:
	TilenetObject();
	virtual ~TilenetObject();

	virtual void destroy() = 0;
	virtual shared_ptr<TilenetObject> clone() = 0;

	template<typename Result>
	std::shared_ptr<Result> self()
	{
		return std::static_pointer_cast<Result>(shared_from_this());
	}

	template<typename Result>
	std::shared_ptr<const Result> self() const
	{
		return std::static_pointer_cast<const Result>(shared_from_this());
	}

	static void Destroy(TilenetObject* obj);
private:
	std::shared_ptr<TilenetObject> mSelf;
};


template<typename T>
class IdObject
{
	typedef TNID							id_type;
	typedef std::vector<weak_ptr<T>>		list_type;
	typedef std::queue<id_type>				queue_type;
public:
	IdObject();
	virtual ~IdObject();

	virtual override void destroy() = 0;
	virtual override shared_ptr<TilenetObject> clone() = 0;

	id_type id() const;

	static id_type Register(const shared_ptr<T>& obj);
	static shared_ptr<T> Resolve(id_type id);
private:
	id_type	mId;
	static list_type			Objects;
	static queue_type			FreeList;
	static std::mutex			IdMutex;
};

template<typename T>
typename IdObject<T>::list_type IdObject<T>::Objects;

template<typename T>
typename IdObject<T>::queue_type IdObject<T>::FreeList;

template<typename T>
std::mutex IdObject<T>::IdMutex;

template<typename T>
IdObject<T>::IdObject()
{
}

template<typename T>
IdObject<T>::~IdObject()
{
	std::lock_guard<std::mutex> lock(IdMutex);
	const id_type rawId = TNEXTRACTID(id());

	tnAssert(Objects.size() > rawId);
	tnAssert(Objects[rawId].lock().get() == this);

	Objects[rawId].reset();
	FreeList.push(rawId);
}

template<typename T>
typename IdObject<T>::id_type IdObject<T>::id() const
{
	return mId;
}

template<typename T>
typename IdObject<T>::id_type IdObject<T>::Register(const shared_ptr<T>& obj)
{
	std::lock_guard<std::mutex> lock(IdMutex);

	id_type newId;
	if(FreeList.size())
	{
		newId = FreeList.front();
		FreeList.pop();

		tnAssert(newId < Objects.size());
		tnAssert(!Objects.at(newId));
		Objects[newId] = obj;
	}else{
		newId = Objects.size();
		Objects.push_back(obj);
	}

	newId |= (std::rand() % TNMAX_IDTAG) << TN_ID_BITLENGTH;
	
	obj->id = newId;

	return newId;
}

template<typename T>
shared_ptr<T> IdObject<T>::Resolve(id_type id)
{
	std::lock_guard<std::mutex> lock(IdMutex);

	id_type rawId = TNEXTRACTID(id);

	if(rawId < Objects.size())
	{
		shared_ptr<T> obj = Objects[rawId].lock();

		if(obj && obj->id() == id)
			return obj;
	}
	
	BOOST_THROW_EXCEPTION(excp::BadIdException() << excp::BadId(*(int*)&id));
}


/// @endcond

#endif