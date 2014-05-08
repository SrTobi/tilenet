#pragma once
#ifndef _TILEY_PLAYER_HPP
#define _TILEY_PLAYER_HPP



#include "tilenet.h"
#include "config.hpp"
#include "layer.hpp"

namespace tiley {


class Participant
	: public AutoObject
	, public AttachSocket<Layer>
{
public:
	inline Participant(TNPARTICIPANT id)
		: mPId(id)
		, mLayerHandle(nullptr)
	{
	}

	inline ~Participant()
	{
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
	inline virtual void on_attached()
	{
		flush();
	}

	inline virtual void do_flush() const
	{
		auto* a = attached()->native_ref();
		if (mLayerHandle != a)
			Impl::AttachLayer(mPId, a);
	}

	inline virtual void apply_to_children(void(*) (Flushable&)) const
	{
		// no children
	}

	inline virtual void lock() const
	{
	}

	inline virtual void unlock() const
	{
	}

private:
	TNPARTICIPANT mPId;
	ObjectHandle mLayerHandle;
};





}

#endif