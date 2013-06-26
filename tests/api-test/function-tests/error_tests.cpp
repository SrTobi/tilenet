#include "test_settings.hpp"
#include "tilenet.h"
#include "tilenet_debug.h"

TILENET_AUTO_TEST_CASE(check_if_dtilenet_is_available)
{
	BOOST_REQUIRE(dtilenet_available() == TNOK);
}


void reset_state()
{
	BOOST_REQUIRE(dtilenet_reset_error_state(TNOK) == TNOK);
}

TILENET_AUTO_TEST_CASE(get_last_error__test)
{
	reset_state();
	BOOST_REQUIRE(tilenet_get_last_error() == TNOK);

	dtilenet_reset_error_state(TNNULLARG);
	BOOST_CHECK(tilenet_get_last_error() == TNNULLARG);


	dtilenet_reset_error_state(TNTIMEOUT);
	BOOST_CHECK(tilenet_get_last_error() != TNTIMEOUT);
}


TILENET_AUTO_TEST_CASE(get_error_string__test)
{
	reset_state();

	const size_t buflen = 1024;
	wchar_t buffer[buflen];

	BOOST_CHECK(tilenet_get_error_string(TNERRI_DESCRIPTION, buffer, buflen) == TNNOERROR);

	dtilenet_reset_error_state(TNBADID);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_DESCRIPTION, 0, 0), TNNULLARG);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_DESCRIPTION, 0, buflen), TNNULLARG);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_DESCRIPTION, buffer, 0), TNNULLARG);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_DESCRIPTION, buffer, 1), TNBUFFERUNDERSIZED);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_BADID, buffer, buflen), TNINFONOTSET);
	BOOST_CHECK_EQUAL(tilenet_get_error_string(TNERRI_DESCRIPTION, buffer, buflen), TNOK);	
}