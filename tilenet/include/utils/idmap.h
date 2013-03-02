#pragma once
#ifndef _IDMAP_H
#define _IDMAP_H

#include "settings.h"

namespace utils {


template<typename T, typename IdT = uint>
class IdMap
{
public:
	typedef T				_value_type;
	typedef IdT				_key_type;

	typedef	std::map<IdT, T>	_containerType;
	typedef IdMap<T>			_type;
	
	
	typedef typename _containerType::iterator		iterator;
	typedef typename _containerType::const_iterator	const_iterator;

public:
	IdMap(uint firstId = 1)
		: mNextId(firstId)
	{
	}


	~IdMap()
	{
	}

	uint nextId() const
	{
		return mNextId;
	}


	void add(const T& e)
	{
		mContainer.insert( std::make_pair(mNextId, e) );
		++mNextId;
	}

	bool get(uint id, T& e) const
	{
		const_iterator it = mContainer.find(id);

		if(it == mContainer.end())
		{
			return false;
		}

		e = it->second;

		return true;
	}

	bool erase(uint id)
	{
		const_iterator it = mContainer.find(id);

		if(it == mContainer.end())
		{
			return false;
		}
		return erase(it);
	}

	bool erase(const const_iterator& it)
	{
		mContainer.erase(it);
		return true;
	}

	uint size() const
	{
		return mContainer.size();
	}

	iterator begin()
	{
		return mContainer.begin();
	}

	iterator end()
	{
		return mContainer.end();
	}

	const_iterator begin() const
	{
		return mContainer.begin();
	}

	const_iterator end() const
	{
		return mContainer.end();
	}

private:
	uint				mNextId;
	_containerType		mContainer;
};




}





#endif
