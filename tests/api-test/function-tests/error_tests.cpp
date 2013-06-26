#include "test_settings.hpp"
#include "tilenet.h"


TILENET_AUTO_TEST_CASE(get_last_error__test)
{
	BOOST_REQUIRE(tilenet_get_last_error() == TNOK);
	BOOST_REQUIRE(tilenet_create_frame(0,0) == TNNULLARG);

	BOOST_CHECK(tilenet_get_last_error() == TNNULLARG);
}