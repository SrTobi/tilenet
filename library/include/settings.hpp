#pragma once
#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

/// @cond DEV

#include <assert.h>
#include <string>
#include <memory>


#include "tilenet.h"

#define OVERRIDE

typedef unsigned int IdType;
typedef std::wstring string;

using std::shared_ptr;
using std::weak_ptr;

#ifdef _DEBUG
#	define TILENET_DEBUG
#endif


#define NOT_IMPLEMENTED()	BOOST_THROW_EXCEPTION(excp::NotImplException());
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

/// @endcond

#endif