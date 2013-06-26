#pragma once
#ifndef _LAYER_HPP
#define _LAYER_HPP

#include <atomic>
#include "settings.hpp"
#include "network/sendable.hpp"

namespace srv {


class Layer
	: public TilenetObject
	, public IdObject<Layer>
{
public:
	typedef shared_ptr<net::Message> Commit;
public:
	Layer();
	~Layer();

	virtual OVERRIDE void destroy();

	TNID currentCommitNr() const;
	virtual std::vector<Commit> getCommitsUpTo(TNID nr) = 0;
	virtual Commit getDelta(TNID nr) = 0;

protected:
	void init();
	TNID newCommit();
private:
	std::atomic<TNID> mCommit;
};



}


#endif
