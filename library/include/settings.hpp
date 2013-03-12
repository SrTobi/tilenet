#pragma once
#ifndef _SETTINGS_HPP
#define _SETTINGS_HPP

#include <assert.h>
#include <string>
#include <boost/shared_ptr.hpp>

#include "tilenet.h"

#define override

typedef unsigned int IdType;
typedef std::wstring string;

#define tnAssert(_expr)		assert(_expr)
#define tilenet_vcast		dynamic_cast

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint32_t uint32;


#include "utils/tilenet_object.hpp"

#endif