#pragma once
#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

/// @cond DEV

#include <assert.h>
#include <string>
#include <memory>
#include <functional>


#include "tilenet.h"

#define OVERRIDE

typedef unsigned int IdType;
typedef std::wstring string;

using std::shared_ptr;
using std::weak_ptr;

#ifdef _DEBUG
#	define TILENET_DEBUG
#endif


#define NOT_IMPLEMENTED()	{assert(!"Not implemented"); BOOST_THROW_EXCEPTION(excp::NotImplException());}
#define IMPLEMENTATION_TODO(_text)	{																\
										static bool BOOST_JOIN(_found_, __LINE__) = false;			\
										if(! BOOST_JOIN(_found_, __LINE__))							\
										{															\
											std::clog << "Not Implemented: "<< _text << std::endl;	\
											BOOST_JOIN(_found_, __LINE__) = true;					\
										}															\
									}


#define tnAssert(_expr)		assert(_expr)
#define tilenet_vcast		dynamic_cast

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint8 byte;


typedef uint8 msgid_type;
typedef uint32 version_type;




#include "utils/exceptions.hpp"
#include "utils/tilenet_object.hpp"
#include "utils/lexical_convert.hpp"
#include "utils/vector.hpp"
#include "utils/log.hpp"


typedef basic_vector2<unsigned int> Point;
typedef basic_vector2<unsigned int> Rect;
typedef basic_vector2<TNRATIO>		Ratio;
typedef basic_vector2<float>		Vector;


template<typename _Ty1, typename _Ty2>
struct std::hash<std::pair<_Ty1, _Ty2> >
{
	std::hash<_Ty1> hash1;
	std::hash<_Ty2> hash2;
public:
	size_t operator()(const std::pair<_Ty1, _Ty2>& p) const throw() {
		return hash1(p.first) + hash2(p.second);
	}
};


/// @endcond

#endif