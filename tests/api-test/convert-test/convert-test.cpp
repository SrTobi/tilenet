#include "tilenet.h"

#include <boost/lexical_cast.hpp>
#include "test_settings.hpp"




/*TILENET_PARAM_TEST_CASE(check_aconvert, const std::string& str)
{

	BOOST_ASSERT(str.size() < 1024);
	std::wstring wstr = boost::lexical_cast<std::wstring>(str);

	wchar_t result[1024];

	tilenet_convert_to_wstring(str.c_str(), result, str.size() + 1);


	BOOST_CHECK(wstr == std::wstring(result));
}*/
/*
TILENET_PARAM_TEST_CASE(check_wconvert, const std::wstring& wstr)
{

	BOOST_ASSERT(wstr.size() < 1024);
	std::string str = boost::lexical_cast<std::string>(wstr);

	char result[1024];

	tilenet_convert_to_astring(wstr.c_str(), result, wstr.size() + 1);


	BOOST_CHECK_EQUAL(str, std::string(result));
}
/*
TILENET_PARAM_TEST(check_aconvert, "hallo");
TILENET_PARAM_TEST(check_aconvert, "tschüss");
TILENET_PARAM_TEST(check_aconvert, "äöü");*/