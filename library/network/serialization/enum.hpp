#pragma once
#ifndef _TN_SERIALIZATION_ENUM_HPP
#define _TN_SERIALIZATION_ENUM_HPP

#include <type_traits>
#include "serialization.hpp"

namespace serialization {


template<typename S, typename E>
void serialize(S& s, E& e, typename std::enable_if<std::is_enum<E>::value>::type* = nullptr)
{
	typename std::underlying_type<E>::type e_extr = e;
	s & e_extr;
	e = E(e_extr);
}




}




#endif