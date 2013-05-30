#pragma once
#ifndef _LEXICAL_CONVERT_HPP
#define _LEXICAL_CONVERT_HPP

#ifdef WIN32
# include <codecvt>
#endif

/// @cond DEV
#include <SFML/System/String.hpp>
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
	return sf::String(from);
}


template<>
inline std::wstring lexical_convert(const std::string& from)
{
	return sf::String(from);
}

template<typename Both>
inline Both lexical_convert(const Both& from)
{
	return from;
}



template<typename Char>
inline std::string to_utf8_string(const std::basic_string<Char>& str)
{
#ifdef WIN32
	std::wstring_convert<std::codecvt_utf8<Char> > converter;
	return converter.to_bytes(str);
#else
	return std::string(str.begin(), str.end());
#endif
}

template<typename Char>
inline std::basic_string<Char> from_utf8_string(const std::string& str)
{
#ifdef WIN32
	std::wstring_convert<std::codecvt_utf8<Char> > converter;
	return converter.from_bytes(str);
#else
	return std::basic_string<Char>(str.begin(), str.end());
#endif
}

/// @endcond
#endif
