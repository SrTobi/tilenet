#include <locale>
#include <assert.h>
#include <hash_map>
#include <boost/thread/tss.hpp>
#include <boost/variant/variant.hpp>

#include "tilenet.h"


struct ThreadErrorInfo
{
	typedef boost::variant<std::wstring, int> value_type;


	ThreadErrorInfo(TNERROR code, const std::wstring& description)
		: errorcode(code)
	{
		infos.insert(std::make_pair(TNERRI_DESCRIPTION, description));
	}


	const TNERROR errorcode;
	std::hash_map<TNERRINFO, value_type> infos;
};

boost::thread_specific_ptr<ThreadErrorInfo> LastThreadError;


void reset_error(TNERROR code, const std::wstring& description)
{
	LastThreadError.reset(new ThreadErrorInfo(code, description));
}

void add_errinfo(TNERRINFO info, int i)
{
	assert(LastThreadError.get());
	assert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, i));
}

void add_errinfo(TNERRINFO info, const std::wstring& str)
{
	assert(LastThreadError.get());
	assert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, str));
}


/**
 * \defgroup Tilenet Global Tilenet functions
 * @{
 */

/**
 * @brief Converts and copies an array of char into an array of wchar_t.
 *
 * \param src An array of char used as source.
 * \param dest An array of wchar_t used as destination.
 * \param size The size of the source string.
 * \return TNOK, if everything worked fine, otherwise TNFAILED
 *
 * \note The size should be also include the zero-termination character!
 * \note The destination array must have an equal or larger then size!
 *
 **/
TNERROR tilenet_convert_to_wstring( const char* src, wchar_t* dest, size_t size )
{
	assert(std::char_traits<char>::length(src) + 1 >= size);

	typedef std::codecvt<wchar_t, char, std::mbstate_t> conv_type;
	
	std::locale loc;
	std::mbstate_t state;
	const conv_type& converter = std::use_facet<conv_type>(loc);
	
	const char* pc;
	wchar_t* pwc;
	conv_type::result result = converter.in(state,
											src, src + size, pc,
											dest, dest + size, pwc);

	if(result != conv_type::ok)
	{
		reset_error(TNFAILED, L"Failed to convert string!");
		return TNFAILED;
	}

	return TNOK;
}


/**
 * @brief Converts and copies an array of wchar_t into an array of char.
 *
 * \param src An array of wchar_t used as source.
 * \param dest An array of char used as destination.
 * \param size The size of the source string.
 * \return TNOK, if everything worked fine, otherwise TNFAILED
 *
 * \note The size should be also include the zero-termination character!
 * \note The destination array must have an equal or larger then size!
 *
 **/
TNERROR tilenet_convert_to_astring( const wchar_t* src, char* dest, size_t size )
{
	assert(std::char_traits<wchar_t>::length(src) + 1 >= size);

	typedef std::codecvt<wchar_t, char, std::mbstate_t> conv_type;

	std::locale loc;
	std::mbstate_t state;
	const conv_type& converter = std::use_facet<conv_type>(loc);

	const wchar_t* pwc;
	char* pc;
	conv_type::result result = converter.out(state,
		src, src + size, pwc,
		dest, dest + size, pc);

	if(result != conv_type::ok)
	{
		reset_error(TNFAILED, L"Failed to convert string!");
		return TNFAILED;
	}

	return TNOK;
}




/**
 * @}
 */