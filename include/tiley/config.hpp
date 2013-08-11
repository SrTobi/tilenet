#pragma once
#ifndef _TILEY_CONFIG_HPP
#define _TILEY_CONFIG_HPP


#define TILEY_OWN_IMPL	0
#define TILEY_STATIC_IMPL 1
#define TILEY_SHARED_IMPL 2

#ifndef TILEY_DEFAULT_IMPL
#	define TILEY_DEFAULT_IMPL TILEY_STATIC_IMPL
#endif

#include "impl/tiley_impl.hpp"

#if TILEY_DEFAULT_IMPL == TILEY_OWN_IMPL
#error Not implemented
#elif TILEY_DEFAULT_IMPL == TILEY_STATIC_IMPL
#	include "impl/static_impl.hpp"

namespace tiley {
	typedef tiley::impl::TileyImpl<tiley::impl::StaticImpl, void> Impl;
}

#elif TILEY_DEFAULT_IMPL == TILEY_STATIC_IMPL
#error Not implemented
#endif



#endif