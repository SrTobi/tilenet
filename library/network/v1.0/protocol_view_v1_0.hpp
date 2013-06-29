#pragma once
#ifndef _PROTOCOL_VIEW_HPP
#define _PROTOCOL_VIEW_HPP


#include "network/serialization/serialization.hpp"
#include "settings.hpp"

namespace proto {
namespace v1_0 {
	

class PView
{
	friend class serialization::access;
public:
	PView();
	PView(const TNVIEW& view);
	~PView();

	bool empty() const;

	const Ratio& outter_posratio() const;
	const Ratio& outter_sizeratio() const;
	const Point& outter_pos() const;
	const Point& outter_size() const;

	const Ratio& inner_posratio() const;
	const Ratio& inner_sizeratio() const;
	const Point& inner_pos() const;
	const Point& inner_size() const;

	const TNCOLOR& color() const;
	TNFLAG flags() const;

private:

	template<typename Archive>
	void serialize(Archive &ar){
		ar & mOutterPosRatio;
		ar & mOutterSizeRatio;
		ar & mOutterPos;
		ar & mOutterSize;

		ar & mInnerPosRatio;
		ar & mInnerSizeRatio;
		ar & mInnerPos;
		ar & mInnerSize;

		ar & mColor & mFlags;
	}

private:
	Ratio mOutterPosRatio;
	Ratio mOutterSizeRatio;
	Point mOutterPos;
	Point mOutterSize;

	Ratio mInnerPosRatio;
	Ratio mInnerSizeRatio;
	Point mInnerPos;
	Point mInnerSize;

	TNCOLOR mColor;
	TNFLAG mFlags;
};






}}







#endif
