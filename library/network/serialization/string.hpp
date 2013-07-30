#pragma once
#ifndef _TN_SERIALIZATION_STRING_HPP
#define _TN_SERIALIZATION_STRING_HPP



#include "utils/lexical_convert.hpp"

namespace serialization {


	
template<typename S>
void save(S& s, const std::string& str)
{
	s << str.size();
	std::copy(str.begin(), str.end(), std::ostream_iterator<char>(s.stream()));
}



template<typename S>
void load(S& s, std::string& str)
{
	std::string::size_type size = 0;
	s >> size;

	if(size == 0)
		return;

	s >> std::noskipws;
	char delmiter;
	s >> delmiter;

	std::copy_n(std::istream_iterator<char>(s.stream()), size, std::back_inserter(str));
}





template<typename S>
void serialize(S& s, std::string& str)
{
	split(s, str);
}







template<typename S>
void save(S& s, const string& str)
{
	s << to_utf8_string(str);
}



template<typename S>
void load(S& s, string& str)
{
	std::string utf8str;
	s >> utf8str;
	str = from_utf8_string<wchar_t>(utf8str);
}


template<typename S>
void serialize(S& s, string& str)
{
	split(s, str);
}


}




#endif
