#pragma once
#ifndef _TILEY_PLAYER_HPP
#define _TILEY_PLAYER_HPP



#include "tilenet.h"
#include "config.hpp"
#include "layer.hpp"

namespace tiley {


class Participant
	: public AutoObject
{
public:
	inline Participant(TNPARTICIPANT id)
		: mPId(id)
		, mAttachedLayer(nullptr)
	{
	}

	inline ~Participant()
	{
		free(mAttachedLayer);
	}


	inline void attach(Layer* layer)
	{
		if(layer)
		{
			free(mAttachedLayer);
			mAttachedLayer = layer;
			Impl::AttachLayer(mPId, layer->native_ref());
		}
	}

	inline void kick()
	{
		Impl::KickParticipant<wchar_t>(mPId, L"no reason");
	}

	template<typename Ch>
	void kick(const std::basic_string<Ch>& reason)
	{
		Impl::KickParticipant(mPId, reason);
	}

	inline TNPARTICIPANT id() const
	{
		return mPId;
	}

	inline TNPARTICIPANT id_index() const
	{
		 return TNEXTRACTID(id());
	}

private:
	TNPARTICIPANT mPId;
	Layer* mAttachedLayer;
};





}

#endif