#pragma once
#ifndef _HELPER_FUNCTIONS_HPP
#define _HELPER_FUNCTIONS_HPP


#include <algorithm>
#include <limits>
#include "settings.hpp"

namespace stdx {


template<typename Iter>
Iter erase_swap(Iter it, Iter end)
{
	tnAssert(it != end);
	end--;

	if(it != end)
		std::swap(*it, *end);

	return end;
}



template<typename Iter, class _Ty>
std::pair<Iter, size_t> swap_remove(Iter begin, Iter end, const _Ty& val, std::size_t max = std::numeric_limits<std::size_t>::max())
{
	std::size_t num = 0;
	while(begin != end && num < max)
	{
		if(*begin == val)
		{
			++num;
			end = erase_swap(begin, end);
		}else
			++begin;
	}

	return std::make_pair(end, num);
}

template<typename Container, class T>
std::size_t swap_remove(Container& cont, const T& val, std::size_t max = std::numeric_limits<std::size_t>::max())
{
	auto end = cont.end();
	auto new_end = end;
	std::size_t num;

	std::tie(new_end, num) = swap_remove(cont.begin(), cont.end(), val, max);
	cont.erase(new_end, end);
	
	return num;
}


}




#endif