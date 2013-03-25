#pragma once
#ifndef _LINK_GRAPH_HPP
#define _LINK_GRAPH_HPP


#include <unordered_map>
#include "settings.hpp"


namespace graph {


template<typename RelatedType, typename TargetType = RelatedType>
class Link
{
public:
	typedef RelatedType related_type;
	typedef TargetType	target_type;

	Link()
		: mSelfPtr(nullptr)
	{
	}

	~Link()
	{
		std::lock_guard<std::mutex> lock(mMutex);

		for(auto& lk : mLinks)
		{
			auto lockedPtr = lk.second.first.lock();

			if(lockedPtr)
			{
				std::lock_guard<std::mutex> lock(lk.second.second->mMutex);

				lk.second.second->mLinks.erase(mSelfPtr);
			}
		}
	}

	void link(Link<target_type, related_type>& other)
	{
		tnAssert(&other != this);
		tnAssert(mRelated.lock());

		std::unique_lock<std::mutex> myLock(mMutex, std::defer_lock);
		std::unique_lock<std::mutex> otherLock(other.mMutex, std::defer_lock);
		std::lock(myLock, otherLock);

		mLinks.insert(std::make_pair(other.mSelfPtr, std::make_pair(other.mRelated, &other)));
		other.mLinks.insert(std::make_pair(mSelfPtr, std::make_pair(mRelated, this)));
	}

	void unlink(std::shared_ptr<TargetType>& ptr)
	{
		unlink(ptr.get());
	}

	void unlink(Link<target_type, related_type>& link)
	{
		unlink(link.mSelfPtr);
	}

	void bind(const shared_ptr<related_type>& related)
	{
		tnAssert(!mRelated.lock());
		mRelated = related;
		mSelfPtr = related.get();
	}

	template<typename T>
	void get(T& cont) const
	{
		std::lock_guard<std::mutex> lock(mMutex);

		for(auto& lk : mLinks)
		{
			auto lockedPtr = lk.second->first.lock();

			if(lockedPtr)
				cont.push_pack(lockedPtr);
		}
	}


	bool has(std::shared_ptr<TargetType>& ptr) const
	{
		return has(ptr.get());
	}

	bool has(Link<target_type, related_type>& link) const
	{
		return has(link.mSelfPtr);
	}

	size_t size() const
	{
		return mLinks.size();
	}

private:
	void unlink(target_type* ptr)
	{
		tnAssert(mRelated.lock());

		shared_ptr<target_type> holder;
		Link<target_type, related_type>* lk;

		{
			std::lock_guard<std::mutex> lock(mMutex);
			auto it = mLinks.find(ptr);
			tnAssert(it != mLinks.end());

			holder = it->second.first.lock();
			lk = it->second.second;

			mLinks.erase(it);
		}

		if(!holder)
			return;
		{
			std::unique_lock<std::mutex> otherLock(lk->mMutex);


			lk->mLinks.erase(mSelfPtr);

		}

	}

	bool has(target_type* ptr) const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		auto it = mLinks.find(ptr);
		
		return it != mLinks.end() && it->second.first.lock();
	}

	related_type*														mSelfPtr;
	weak_ptr<related_type>												mRelated;
	mutable std::mutex													mMutex;
	std::unordered_map<target_type*, std::pair<weak_ptr<target_type>, Link<target_type, related_type>*>>	mLinks;
};





}




#endif