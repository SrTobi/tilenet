#pragma once
#ifndef _UTF8_STRING_HPP
#define _UTF8_STRING_HPP

#include <boost/serialization/access.hpp>
#include "settings.hpp"

class utf8_string
{
	friend class boost::serialization::access;
public:
	utf8_string();
	utf8_string(const string& str);
	~utf8_string();

	utf8_string& operator = (const string& str);
	operator string() const; 

private:
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & mUtf8String;
	}

	std::string mUtf8String;
};


template<typename Char>
std::basic_ostream<Char>& operator << (std::basic_ostream<Char>& stream, const utf8_string& str)
{
	stream << (string)str;
	return stream;
}




#endif
