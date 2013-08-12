#pragma once
#ifndef _FUNCCTIONS_HPP
#define _FUNCCTIONS_HPP


#include <chrono>
#include "config.hpp"


namespace tiley {


inline void WaitForClientToExit()
{
	Impl::Exit();
}

template<typename Rep, typename Period>
inline void WaitForClientToExit(std::chrono::duration<Rep,Period>& timeout_duration)
{
	using std::chrono::duration_cast;
	typedef std::chrono::milliseconds target_time_format;
	typedef std::chrono::duration<Rep,Period> src_time_format;

	size_t timeout = duration_cast<target_time_format>(timeout_duration).count();
	Impl::Exit(&timeout);

	timeout_duration = target_time_format(timeout);
}


template<typename Rep, typename Period>
inline void WaitForClientToExit(const std::chrono::duration<Rep,Period>& timeout_duration)
{
	auto dummy = timeout_duration;
	WaitForClientToExit(dummy);
}

}

#endif