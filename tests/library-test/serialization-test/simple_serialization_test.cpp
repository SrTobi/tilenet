#include "includes.hpp"


#include "test_settings.hpp"
#include "network/serialization/serialization.hpp"
#include "network/serialization/vector.hpp"


struct serializable_test_struct
{
	friend class serialization::access;
	int x1;
	float x2;
	std::string x3;

private:
	template<typename S>
	void serialize(S& s)
	{
		s & x1 & x2 & x3;
	}
};

static_assert(serialization::access::has_serialize_function<serializable_test_struct>::value, "has_serialize_function is corrupted");
static_assert(!serialization::access::has_serialize_function<int>::value, "has_serialize_function is corrupted");

TILENET_AUTO_TEST_CASE(simple_serialization)
{
	const serializable_test_struct astruct = { 1, 2.5f, "test wie gehts" };
	std::string buffer;

	{
		std::ostringstream oss;
		serialization::TextSerializer<> seri(oss);

		seri << astruct;

		buffer = oss.str();
	}

	{
		std::istringstream iss(buffer);
		serialization::TextDeserializer<> deseri(iss);

		serializable_test_struct s;
		deseri >> s;

		BOOST_CHECK_EQUAL(astruct.x1, s.x1);
		BOOST_CHECK_CLOSE_FRACTION(astruct.x2, s.x2, 0.001f);
		BOOST_CHECK_EQUAL(astruct.x3, s.x3);
	}
}

struct free_serializable_test_struct
{
	int x1;
	float x2;
};

namespace serialization {

	template<typename S>
	void save(S& seri, const free_serializable_test_struct& s)
	{
		seri << s.x1 << s.x2;
	}

	template<typename S>
	void load(S& seri, free_serializable_test_struct& s)
	{
		seri >> s.x1 >> s.x2;
	}

	template<typename S>
	void serialize(S& seri, free_serializable_test_struct& s)
	{
		split(seri, s);
	}
}


TILENET_AUTO_TEST_CASE(simple_free_serialization)
{
	const free_serializable_test_struct astruct = { 1, 2.5f };
	std::string buffer;

	{
		std::ostringstream oss;
		serialization::TextSerializer<> seri(oss);

		seri << astruct;

		buffer = oss.str();
	}

	{
		std::istringstream iss(buffer);
		serialization::TextDeserializer<> deseri(iss);

		free_serializable_test_struct s;
		deseri >> s;

		BOOST_CHECK_EQUAL(astruct.x1, s.x1);
		BOOST_CHECK_CLOSE_FRACTION(astruct.x2, s.x2, 0.001f);
	}
}


TILENET_AUTO_TEST_CASE(simple_vector_serialization)
{
	std::vector<std::string> vec;
	vec.push_back(" test ");
	vec.push_back(" test_1 ");
	vec.push_back(" test  huhu ");
	std::string buffer;

	{
		std::ostringstream oss;
		serialization::TextSerializer<> seri(oss);

		seri << vec;

		buffer = oss.str();
	}

	{
		std::istringstream iss(buffer);
		serialization::TextDeserializer<> deseri(iss);

		std::vector<std::string> v;
		deseri >> v;

		BOOST_CHECK_EQUAL_COLLECTIONS(vec.begin(), vec.end(), v.begin(), v.end());
	}
}