#include "includes.hpp"


#include "test_settings.hpp"
#include "network/protocol_deserializer.hpp"




BOOST_AUTO_TEST_CASE(simple_primitive_deserializer_test1)
{
	std::string input = "1234";
	int i;
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		d >> i;
	}

	BOOST_CHECK_EQUAL(1234, i);
}


BOOST_AUTO_TEST_CASE(simple_primitive_deserializer_test2)
{
	std::string input = "1234 4321";
	int i1, i2;
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		d >> i1 >> i2;
	}

	BOOST_CHECK_EQUAL(1234, i1);
	BOOST_CHECK_EQUAL(4321, i2);
}


BOOST_AUTO_TEST_CASE(simple_sequence_deserializer_test1)
{
	std::string input = "{1,2,3,4}";
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		net::SequenceExtractor<std::string::const_iterator> seq(d);

		int i = 1;
		for(auto& e : seq)
		{
			int extr;
			e >> extr;
			BOOST_CHECK_EQUAL(extr, i);

			++i;
		}
	}
}


BOOST_AUTO_TEST_CASE(simple_sequence_deserializer_test2)
{
	std::string input = "{1 4,2 3,3 2,4 1}";
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		net::SequenceExtractor<std::string::const_iterator> seq(d);

		int i = 1;
		for(auto& e : seq)
		{
			int extr1, extr2;
			e >> extr1 >> extr2;
			BOOST_CHECK_EQUAL(extr1, i);
			BOOST_CHECK_EQUAL(extr2, 5 - i);

			++i;
		}
	}
}



BOOST_AUTO_TEST_CASE(simple_string_deserializer_test1)
{
	std::wstring expected = L"test string";
	std::string input = "\"test string\"";

	std::wstring extr;
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		d >> extr;
	}

	BOOST_CHECK(extr == expected);
}


BOOST_AUTO_TEST_CASE(simple_string_deserializer_test2)
{
	std::string input = "\"test string\" \"---\"";

	std::wstring extr1, extr2;
	{
		net::ProtocolDeserializer<std::string::const_iterator>::range_type input_range(input.begin(), input.end());
		net::ProtocolDeserializer<std::string::const_iterator> d(input_range);

		d >> extr1 >> extr2;
	}

	BOOST_CHECK(extr1 == L"test string");
	BOOST_CHECK(extr2 == L"---");
}