#pragma once
#ifndef _TN_SERIALIZATION_PAIR_HPP
#define _TN_SERIALIZATION_PAIR_HPP


#include "serialization.hpp"

namespace serialization {



template<typename S, typename Ty1, typename Ty2>
void serialize(S& s, std::pair<Ty1, Ty2>& p)
{
	s & p.first & p.second;
}




}




#endif