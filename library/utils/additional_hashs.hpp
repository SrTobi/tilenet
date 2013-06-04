#pragma once
#ifndef _ADDITIONAL_HASHS
#define _ADDITIONAL_HASHS

#include <boost/functional/hash/hash.hpp>
#include "settings.hpp"

namespace std {
	
template<typename _Ty1, typename _Ty2>
struct hash<std::pair<_Ty1, _Ty2> >
{
	std::hash<_Ty1> hash1;
	std::hash<_Ty2> hash2;
public:
	size_t operator()(const std::pair<_Ty1, _Ty2>& p) const throw() {
		return hash1(p.first) + hash2(p.second);
	}
};


template<>
struct hash<fs::path>
{
public:
	size_t operator()(const fs::path& path) const throw() {
		return fs::hash_value(path);
	}
};

}


#endif
