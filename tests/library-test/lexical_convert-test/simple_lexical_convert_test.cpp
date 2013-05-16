#include "includes.hpp"


#include "test_settings.hpp"
#include "utils/lexical_convert.hpp"



BOOST_AUTO_TEST_CASE(empty_wstring_to_string)
{
	std::wstring wstr = L"";
	std::string str = lexical_convert<std::string>(wstr);
}

BOOST_AUTO_TEST_CASE(empty_string_to_wstring)
{
	std::string str = "";
	std::wstring wstr = lexical_convert<std::wstring>(str);
}

