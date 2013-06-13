#pragma once
#ifndef _COMMIT_QUEUE_HPP
#define _COMMIT_QUEUE_HPP

#include <map>
#include "settings.hpp"
#include "network/message.hpp"


namespace srv {

class CommitQueue
{
	typedef shared_ptr<net::Message> commit_type;
	typedef std::pair<commit_type, commit_type> commit_slot;
	typedef std::map<TNID, commit_slot> storage_type;
public:
	CommitQueue(TNID firstKey);

	void commit(TNID key, const commit_type& val);
	void commitDelta(TNID key, const commit_type& val);
	commit_type getDelta(TNID key) const;
	std::vector<commit_type> getCommitsUpTo(TNID key) const;

private:
	commit_type _lastFullCommit() const;
	void _check_size();

private:
	TNID mNextKey;
	storage_type mCommits;

	std::size_t mSizeSinceNonDelta;
	storage_type::const_iterator mLastNonDelta;
};



}


#endif
