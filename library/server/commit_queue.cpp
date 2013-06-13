#include "includes.hpp"
#include "commit_queue.hpp"


namespace srv {


#define MAX_COMMITS 30

CommitQueue::CommitQueue(TNID firstKey)
	: mNextKey(firstKey)
	, mSizeSinceNonDelta(0)
	, mLastNonDelta(mCommits.end())
{
}

void CommitQueue::commit(TNID key, const commit_type& val)
{
	tnAssert(key <= mNextKey);

	if(key == mNextKey)
	{
		mCommits.clear();
		mCommits.emplace(key, commit_slot(nullptr, val));
		mLastNonDelta = mCommits.begin();
		mSizeSinceNonDelta = 0;
		++mNextKey;
	}else{
		assert(mLastNonDelta != mCommits.end());

		// Check if a newer full commit exists
		if(mLastNonDelta->first < key)
		{
			auto it = mCommits.find(key);
			assert(it != mCommits.end());

			auto& v = it->second;
			v.second = val;
			mLastNonDelta = it;

			// calculate mSizeSinceNonDelta
			mSizeSinceNonDelta = 0;
			for(; it != mCommits.end(); ++it)
			{
				auto delta = it->second.first;
				assert(delta);
				mSizeSinceNonDelta += delta->size();
			}
		}
		_check_size();
	}
}

void CommitQueue::commitDelta(TNID key, const commit_type& val)
{
	// Deltas needs to be committed in the right order
	assert(mNextKey == key);
	++mNextKey;

	mCommits.emplace_hint(mCommits.end(), key, commit_slot(val, nullptr));
	mSizeSinceNonDelta += val->size();

	_check_size();
}

CommitQueue::commit_type CommitQueue::getDelta(TNID key) const
{
	auto it = mCommits.find(key);

	if(it == mCommits.end())
		return nullptr;

	return it->second.first? it->second.first : it->second.second;
}

std::vector<CommitQueue::commit_type> CommitQueue::getCommitsUpTo( TNID key ) const
{
	std::vector<commit_type> result;

	if(mSizeSinceNonDelta < _lastFullCommit()->size() && key >= mLastNonDelta->first)
	{
		auto end = mCommits.find(key);

		for(auto it = mLastNonDelta; it != end; ++it)
		{
			result.emplace_back(it->second.first);
		}

		result.front() = mLastNonDelta->second.second;
	}

	return result;
}

CommitQueue::commit_type CommitQueue::_lastFullCommit() const
{
	auto c = mLastNonDelta->second.second;
	assert(c);
	return c;
}

void CommitQueue::_check_size()
{
	if(mCommits.size() > MAX_COMMITS)
	{
		for(auto it = mCommits.begin(); it != mLastNonDelta;)
		{
			assert(it != mCommits.end());
			it = mCommits.erase(it);
		}
	}
}


}