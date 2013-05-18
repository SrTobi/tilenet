#include "includes.hpp"


#include "test_settings.hpp"
#include "network/protocol_serializer.hpp"




BOOST_AUTO_TEST_CASE(simple_primitive_serializer_test1)
{
	std::string expected = "5123";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << 5123;
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}


BOOST_AUTO_TEST_CASE(simple_primitive_serializer_test2)
{
	std::string expected = "5123 -123";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << 5123 << -123;
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}


BOOST_AUTO_TEST_CASE(simple_string_serializer_test1)
{
	std::wstring expected = L"\"test\"";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << std::wstring(L"test");
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}



BOOST_AUTO_TEST_CASE(simple_string_serializer_test2)
{
	std::wstring expected = L"\"\\\"test\\\"\"";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << std::wstring(L"\"test\"");
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}


BOOST_AUTO_TEST_CASE(simple_sequence_serializer_test1)
{
	std::wstring expected = L"123 {0,1,2,3,4}";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << 123;
		{
			net::SequenceInserter<> seq(s);

			for(unsigned int i = 0; i < 5; ++i)
			{
				auto& e = seq.newElement();
				e << i;
			}
		}
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}


BOOST_AUTO_TEST_CASE(simple_sequence_serializer_test2)
{
	std::wstring expected = L"111 {0 \"---\",1 \"---\"}";
	std::vector<char> buffer;
	{
		net::ProtocolSerializer<> s(buffer);

		s << 111;
		{
			net::SequenceInserter<> seq(s);

			for(unsigned int i = 0; i < 2; ++i)
			{
				auto& e = seq.newElement();
				e << i;
				e << L"---";
			}
		}
	}

	BOOST_CHECK(std::equal(buffer.begin(), buffer.end(), expected.begin()));
}
