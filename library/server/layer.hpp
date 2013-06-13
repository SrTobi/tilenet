#pragma once
#ifndef _LAYER_HPP
#define _LAYER_HPP

#include <atomic>
#include "settings.hpp"
#include "network/sendable.hpp"

namespace srv {

class Frame;

class Layer
	: public TilenetObject
	, public IdObject<Layer>
{
public:
	typedef shared_ptr<net::Message> Commit;
public:
	Layer();
	~Layer();

	virtual OVERRIDE void destroy() = 0;
	virtual OVERRIDE shared_ptr<TilenetObject> clone() = 0;

	TNID currentCommitNr() const;
	virtual std::vector<Commit> getCommitsUpTo(TNID nr) = 0;
	virtual Commit getDelta(TNID nr) = 0;

protected:
	TNID newCommit();
private:
	std::atomic<TNID> mCommit;
};



}


#endif
