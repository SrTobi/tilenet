#pragma once
#ifndef _LEXICAL_CONVERT_HPP
#define _LEXICAL_CONVERT_HPP

/// @cond DEV

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

	typedef std::codecvt<char, wchar_t, std::mbstate_t> conv_type;

	std::locale loc;
	std::mbstate_t state;
	const conv_type& converter = std::use_facet<conv_type>(loc);

	const wchar_t* src = &from[0];
	char* dest = &to[0];

	const wchar_t* pc;
	char* pwc;
	conv_type::result result = converter.in(state,
		src, src + from.size(), pc,
		dest, dest + from.size(), pwc);

	if(result != conv_type::ok)
	{
		throw std::bad_cast();
	}

	return std::move(to);
}


template<>
inline std::wstring lexical_convert(const std::string& from)
{
	std::wstring to(from.size(), ' ');

	typedef std::codecvt<wchar_t, char, std::mbstate_t> conv_type;

	std::locale loc;
	std::mbstate_t state;
	const conv_type& converter = std::use_facet<conv_type>(loc);

	const char* src = &from[0];
	wchar_t* dest = &to[0];

	const char* pc;
	wchar_t* pwc;
	conv_type::result result = converter.in(state,
		src, src + from.size(), pc,
		dest, dest + from.size(), pwc);

	if(result != conv_type::ok)
	{
		throw std::bad_cast();
	}

	return std::move(to);
}

template<typename Both>
inline Both lexical_convert(const Both& from)
{
	return from;
}


/// @endcond
#endif
