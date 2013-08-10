#pragma once
#ifndef _TILEY_HPP
#define _TILEY_HPP

#define NO_IMPL		"impl/no_impl.hpp"
#define STATIC_IMPL "impl/static_impl.hpp""
#define SHARED_IMPL "impl/shared_impl.hpp"

#ifndef TILEY_DEFAULT_IMPL
#	define TILEY_DEFAULT_IMPL STATIC_IMPL
#endif

#include TILEY_DEFAULT_IMPL






#endif