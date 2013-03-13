#include "includes.hpp"

#include <locale>
#include <assert.h>
#include <hash_map>
#include <boost/thread/tss.hpp>
#include <boost/variant/variant.hpp>

#include "tilenet.h"
#include "settings.hpp"

struct ThreadErrorInfo
{
	typedef boost::variant<string, int> value_type;


	ThreadErrorInfo(TNERROR code, const string& description)
		: errorcode(code)
	{
		infos.insert(std::make_pair(TNERRI_DESCRIPTION, description));
	}


	const TNERROR errorcode;
	std::hash_map<TNERRINFO, value_type> infos;
};

boost::thread_specific_ptr<ThreadErrorInfo> LastThreadError;


void add_errinfo(TNERRINFO info, int i)
{
	tnAssert(LastThreadError.get());
	tnAssert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, i));
}

void add_errinfo(TNERRINFO info, const string& str)
{
	tnAssert(LastThreadError.get());
	tnAssert(LastThreadError->infos.count(info) == 0);
	LastThreadError->infos.insert(std::make_pair(info, str));
}

void reset_error(TNERROR code, const string& description)
{
	LastThreadError.reset(new ThreadErrorInfo(code, description));
}


void reset_error(const excp::CodeException& exp)
{
	reset_error(exp.getErrorCode(), exp.why());
}

template<typename Type>
class VisitorForInfoType{};

template<>
class VisitorForInfoType<int> : public boost::static_visitor<int>
{
public:
	int operator()(int i)				const { return i; }
	int operator()(const string& str)	const { BOOST_THROW_EXCEPTION(excp::WrongInfoTypeException()); }
};

template<>
class VisitorForInfoType<string> : public boost::static_visitor<string>
{
public:
	string operator()(int i)				const { BOOST_THROW_EXCEPTION(excp::WrongInfoTypeException()); }
	string operator()(const string& str)	const { return str; }
};

template<typename Target>
Target get_error(TNERRINFO code)
{
	auto it = LastThreadError->infos.find(code);

	if(it == LastThreadError->infos.end())
		BOOST_THROW_EXCEPTION(excp::InfoNotSetException() << excp::InfoCode(code));

	return boost::apply_visitor(VisitorForInfoType<Target>(), it->second);
}


void copy_string(const string& src, wchar_t* dest, size_t buflen)
{
	size_t len = std::min(src.size(), buflen-1);

	memcpy(dest, src.c_str(), len * sizeof(wchar_t));
	dest[len] = L'\0';

	if(len < src.size())
		BOOST_THROW_EXCEPTION(excp::BufferUndersizedException() << excp::CopiedEements(len+1));
}



#define CHECK_RETURN(_cond, _ret)	if(!(_cond)){return (_ret);}
#define CHECK_NULL(_expr)			CHECK_RETURN(_expr, TNNULLARG)


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
	CHECK_NULL(src);
	CHECK_NULL(dest);
	CHECK_NULL(size);

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
		reset_error(TNUNKNOWN, L"Failed to convert string!");
		return TNUNKNOWN;
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
 * \note The destination array must have an equal or larger size!
 *
 **/
TNERROR tilenet_convert_to_astring( const wchar_t* src, char* dest, size_t size )
{
	CHECK_NULL(src);
	CHECK_NULL(dest);
	CHECK_NULL(size);

	tnAssert(std::char_traits<wchar_t>::length(src) + 1 >= size);

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
		reset_error(TNUNKNOWN, L"Failed to convert string!");
		return TNUNKNOWN;
	}

	return TNOK;
}

/**
 * @brief Returns an error info, which is a string
 *
 * \param infono The number, identifying the certain info
 * \param dest A buffer used as destination
 * \param buflen Size of the buffer indicating the maximum size of the output string
 * \return TNOK, if info was copied successfully
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is an int
 * \return TNBUFFEROVERFLOW, if buflen was smaller then the string which should be copied!
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen )
{
	CHECK_NULL(dest);
	CHECK_NULL(buflen);

	try {
		string infoString = get_error<string>(infono);
		copy_string(infoString, dest, buflen);

		return TNOK;

	} catch(excp::InfoNotSetException&)
	{
		return TNINFONOTSET;
	
	} catch(excp::WrongInfoTypeException&)
	{
		return TNWRONGINFOTYPE;
	
	} catch(excp::BufferUndersizedException&)
	{
		return TNBUFFERUNDERSIZED;

	} catch(...)
	{
		return TNUNKNOWN;
	}
}

/**
 * @brief Returns an error info, which is an int
 *
 * \param infono The number, identifying the certain info
 * \param dest A buffer used as destination
 * \param buflen Size of the buffer indicating the maximum size of the output string
 * \return TNOK, if info was copied successfully
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is a string
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest )
{
	CHECK_NULL(dest);

	try {
		*dest = get_error<int>(infono);

		return TNOK;

	} catch(excp::InfoNotSetException&)
	{
		return TNINFONOTSET;

	} catch(excp::WrongInfoTypeException&)
	{
		return TNWRONGINFOTYPE;

	} catch(...)
	{
		return TNUNKNOWN;
	}
}

TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied )
{
	NOT_IMPLEMENTED();
}




/**
 * @}
 */