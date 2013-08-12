#pragma once
#ifndef _TILEY_CHAR_CAST_HPP
#define _TILEY_CHAR_CAST_HPP

#include <string>

namespace tiley {


class BasicCharCaster
{
public:

	inline const wchar_t* c_wstr(const char* str)
	{
		wchar_t* dest = buf;
		const char* src = str;

		// TODO do it better (i know this isn't working all the time
		do {
			*dest = *src;

			++dest;
		}while(*(src++));

		return buf;
	}

	inline const wchar_t* c_wstr(const std::string& str)
	{
		return c_wstr(str.c_str());
	}

	inline const wchar_t* c_wstr(const wchar_t* str)
	{
		return str;
	}

	inline const wchar_t* c_wstr(const std::wstring& str)
	{
		return str.c_str();
	}
	
	template<typename Ch>
	inline void str(std::basic_string<Ch>& target, const wchar_t* cstr)
	{
		while(*cstr)
			target.push_back(*(cstr++));
	}

private:
	wchar_t buf[1024];
};




}



#endif
