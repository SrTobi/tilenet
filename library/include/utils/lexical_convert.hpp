#pragma once
#ifndef _LEXICAL_CONVERT_HPP
#define _LEXICAL_CONVERT_HPP

#include <boost/lexical_cast.hpp>
#include "settings.hpp"



template<typename To, typename From>
inline To lexical_convert(const From& from)
{
	static_assert(sizeof(To) != sizeof(To), "Can not convert 'From' to 'To'!");
}

template<typename To>
inline To lexical_convert(const char* from)
{
	return lexical_convert<To>(std::string(from));
}


template<typename To>
inline To lexical_convert(const wchar_t* from)
{
	return lexical_convert<To>(std::wstring(from));
}

template<>
inline std::string lexical_convert(const std::wstring& from)
{
	std::string to(from.size(), ' ');
	tilenet_convert_to_astring(from.data(), &to[0], from.size());
	return std::move(to);
}


template<>
inline std::wstring lexical_convert(const std::string& from)
{
	std::wstring to(from.size(), ' ');
	tilenet_convert_to_wstring(from.data(), &to[0], from.size());
	return std::move(to);
}

template<typename Both>
inline Both lexical_convert(const Both& from)
{
	return from;
}

#endif
