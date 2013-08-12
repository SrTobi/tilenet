#pragma once
#ifndef _TILEY_ERROR_HPP
#define _TILEY_ERROR_HPP

#include <exception>
#include <string>

#include "impl/char_cast.hpp"

namespace tiley {


class TileyException: public std::exception
{
public:
	inline TileyException()
		: mWhat("Unknown error!")
	{
	}

	inline TileyException(const std::string& what)
		: mWhat(what)
	{
	}

	inline TileyException(const std::wstring& what)
	{
	}

	inline virtual const char* what() const
	{
		return mWhat.c_str();
	}

	inline virtual const std::string& why() const
	{
		return mWhat;
	}

	inline virtual std::wstring wwhy() const
	{
		return L"";
	}

private:
	std::string mWhat;
};





}

#endif