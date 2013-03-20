#pragma once
#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

/// @cond DEV

#include <assert.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>


#include "tilenet.h"

#define override

typedef unsigned int IdType;
typedef std::wstring string;

using boost::shared_ptr;
using boost::make_shared;
using boost::weak_ptr;

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





#include "utils/exceptions.hpp"
#include "utils/tilenet_object.hpp"
#include "utils/lexical_convert.hpp"

/// @endcond

#endif