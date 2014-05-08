#pragma once
#ifndef _TILEY_CHAR_CAST_HPP
#define _TILEY_CHAR_CAST_HPP

#include <string>

namespace tiley {
	
template<typename Ch>
class ChooseCStr
{};

template<>
struct ChooseCStr<char>
{
	static inline const char* choose(const char* c, const wchar_t* wc) { return c;  }
};

template<>
struct ChooseCStr<wchar_t>
{
	static inline const wchar_t* choose(const char* c, const wchar_t* wc) { return wc; }
};

#define CSTR_T(_ch, _str) (ChooseCStr<_ch>::choose(_str, L##_str))

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
