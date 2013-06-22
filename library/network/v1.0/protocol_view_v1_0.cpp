#include "includes.hpp"
#include "protocol_view_v1_0.hpp"


namespace proto {
namespace v1_0 {



PView::PView()
	: mColor(0) // <- set alpha to 0 to indicate an empty view
{
}

PView::PView( const TNVIEW& view)
	: mOutterPosRatio(view.outter_xr, view.outter_yr)
	, mOutterSizeRatio(view.outter_wr, view.outter_hr)
	, mOutterPos(view.outter_x, view.outter_y)
	, mOutterSize(view.outter_w, view.outter_h)

	, mInnerPosRatio(view.inner_xr, view.inner_yr)
	, mInnerSizeRatio(view.inner_wr, view.inner_hr)
	, mInnerPos(view.inner_x, view.inner_y)
	, mInnerSize(view.inner_w, view.inner_h)

	, mColor(view.color)
	, mFlags(view.flags)
{
}

PView::~PView()
{
}

bool PView::empty() const
{
	return TNGET_ALPHA(mColor) == 0;
}

const Ratio& PView::outter_posratio() const
{
	tnAssert(!empty());
	return mOutterPosRatio;
}

const Ratio& PView::outter_sizeratio() const
{
	tnAssert(!empty());
	return mOutterSizeRatio;
}

const Point& PView::outter_pos() const
{
	tnAssert(!empty());
	return mOutterPos;
}

const Point& PView::outter_size() const
{
	tnAssert(!empty());
	return mOutterSize;
}

const Ratio& PView::inner_posratio() const
{
	tnAssert(!empty());
	return mInnerPosRatio;
}

const Ratio& PView::inner_sizeratio() const
{
	tnAssert(!empty());
	return mInnerSizeRatio;
}

const Point& PView::inner_pos() const
{
	tnAssert(!empty());
	return mInnerPos;
}

const Point& PView::inner_size() const
{
	return mInnerSize;
}

const TNCOLOR& PView::color() const
{
	tnAssert(!empty());
	return mColor;
}

TNFLAG PView::flags() const
{
	tnAssert(!empty());
	return mFlags;
}



}}