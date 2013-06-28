#include "includes.hpp"
#include "utf8_string.hpp"

#include "lexical_convert.hpp"


utf8_string::utf8_string()
{
}

utf8_string::utf8_string( const string& str )
	: mUtf8String(to_utf8_string(str))
{
}

utf8_string::~utf8_string()
{

}

utf8_string& utf8_string::operator=( const string& str )
{
	mUtf8String = to_utf8_string(str);
	return *this;
}

utf8_string::operator string() const
{
	return from_utf8_string<wchar_t>(mUtf8String);
}
