#include "includes.hpp"


#include "test_settings.hpp"
#include "network/protocol_deserializer.hpp"




BOOST_AUTO_TEST_CASE(simple_primitive_deserializer_test1)
{
	std::string input = "1234";
	int i;
	{
		net::ProtocolDeserializer<std::string::const_iterator> d(input.begin(), input.end());

		d >> i;
	}

	BOOST_CHECK_EQUAL(1234, i);
}


BOOST_AUTO_TEST_CASE(simple_primitive_deserializer_test2)
{
	std::string input = "1234 4321";
	int i1, i2;
	{
		net::ProtocolDeserializer<std::string::const_iterator> d(input.begin(), input.end());

		d >> i1 >> i2;
	}

	BOOST_CHECK_EQUAL(1234, i1);
	BOOST_CHECK_EQUAL(4321, i2);
}
