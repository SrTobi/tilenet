#include <locale>
#include <assert.h>
#include <boost/thread/tss.hpp>

#include "tilenet.h"



TNERROR tilenet_convert_to_wstring( const char* src, wchar_t* dest, size_t size )
{
	assert(strlen(src) >= size);

	typedef std::codecvt<wchar_t, char, std::mbstate_t> conv_type;
	
	std::locale loc;
	std::mbstate_t state;
	const conv_type& converter = std::use_facet<conv_type>(loc);
	
	const char* pc;
	wchar_t* pwc;
	conv_type::result result = converter.in(state,
											src, src + size, pc,
											dest, dest + size, pwc);

	return result == conv_type::ok? TNOK : TNFAILED;
}
