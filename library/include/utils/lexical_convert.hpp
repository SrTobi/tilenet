#pragma once
#ifndef _LEXICAL_CONVERT_HPP
#define _LEXICAL_CONVERT_HPP

#include "settings.hpp"



template<typename To, typename From>
static To lexical_convert(const From& from)
{
	static_assert(sizeof(To) != sizeof(To), "Can not convert 'From' to 'To'!");
}

template<typename To>
static To lexical_convert(const char* from)
{
	return lexical_convert<To>(std::string(from));
}


template<typename To>
static To lexical_convert(const wchar_t* from)
{
	return lexical_convert<To>(std::wstring(from));
}

template<>
static std::string lexical_convert(const std::wstring& from)
{
	std::string to(from.size(), ' ');
	tilenet_convert_to_astring(from.data(), &to[0], from.size());
	return std::move(to);
}


template<>
static std::wstring lexical_convert(const std::string& from)
{
	std::wstring to(from.size(), ' ');
	tilenet_convert_to_wstring(from.data(), &to[0], from.size());
	return std::move(to);
}

template<typename Both>
static Both lexical_convert(const Both& from)
{
	return from;
}

#endif