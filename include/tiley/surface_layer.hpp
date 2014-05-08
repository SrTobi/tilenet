#pragma once
#ifndef _SURFACE_LAYER_HPP
#define _SURFACE_LAYER_HPP

#include "impl/char_cast.hpp"
#include "attachable.hpp"
#include "surface.hpp"
#include "tn_object.hpp"

namespace tiley {

template<typename Ch = TILEY_DEFAULT_CHAR>
class SurfaceLayer
	: public Layer
	, public AttachSocket<Surface>
{
public:
	typedef std::basic_string<Ch> string_type;
	typedef Surface::size_type size_type;
	typedef Ratio ratio_type;
public:
	SurfaceLayer(const ratio_type& ratio = ratio_type(1.0f, 1.0f), TNFLAG flags = 0)
		: mAspect(CSTR_T(Ch, "std"))
		, mRatio(ratio)
		, mFlag(flags)
		, mNeedNewLayer(true)
	{
	}

	SurfaceLayer(const string_type& aspect, const ratio_type& ratio = ratio_type(1.0f, 1.0f), TNFLAG flags = 0)
		: mAspect(aspect)
		, mRatio(ratio)
		, mFlag(flags)
		, mNeedNewLayer(true)
	{
	}

	~SurfaceLayer()
	{
	}

	Surface* attached_surface()
	{
		return attached();
	}

	const Surface* attached_surface() const
	{
		return attached();
	}


protected:
	virtual void on_attached()
	{
		mNeedNewLayer = true;
	}

	virtual void do_flush() const
	{
		if (!attached_surface())
			return;

		if (mNeedNewLayer)
		{
			mNeedNewLayer = false;
			mObject.reset(Impl::CreateTilelayer(attached_surface()->size(), mRatio, mAspect, mFlag));
		}

		Impl::UpdateTilelayer(mObject.native_ref(), attached_surface()->content(), attached_surface()->updated_content());
		attached_surface()->clear_update();
	}

	virtual void apply_to_children(void(*) (Flushable&)) const
	{
		// no children
	}


	virtual void lock() const
	{
	}

	virtual void unlock() const
	{
	}

private:
	ratio_type mRatio;
	TNFLAG mFlag;
	string_type mAspect;
	mutable TNObject mObject;
	mutable bool mNeedNewLayer;
};



}

#endif