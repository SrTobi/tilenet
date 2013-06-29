#pragma once
#ifndef _TN_SERIALIZATION_WCHAR_HPP
#define _TN_SERIALIZATION_WCHAR_HPP


#include "serialization.hpp"

namespace serialization {


template<typename S>
void serialize(S& s, wchar_t& wch)
{
	uint32 v = wch;
	s & v;
	wch = v;
}




}




#endif