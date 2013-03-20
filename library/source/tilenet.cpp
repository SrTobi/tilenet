#include "includes.hpp"

#include <locale>
#include <assert.h>
#include <boost/thread/tss.hpp>
#include <boost/unordered_map.hpp>
#include <boost/variant/variant.hpp>

#include "tilenet.h"
#include "settings.hpp"


/// @cond DEV

struct ThreadErrorInfo
{
	typedef boost::variant<string, int> value_type;


	ThreadErrorInfo(TNERROR code, const string& description)
		: errorcode(code)
	{
		infos.insert(std::make_pair(TNERRI_DESCRIPTION, description));
	}


	const TNERROR errorcode;
	boost::unordered_map<TNERRINFO, value_type> infos;
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
	if(!LastThreadError.get())
		BOOST_THROW_EXCEPTION(excp::NoErrorException());

	auto it = LastThreadError->infos.find(code);

	if(it == LastThreadError->infos.end())
		BOOST_THROW_EXCEPTION(excp::InfoNotSetException() << excp::InfoCode(code));

	return boost::apply_visitor(VisitorForInfoType<Target>(), it->second);
}

#define EXTRACT_ERROR_INFO(_expType)									\
	{																	\
		auto const*const p = boost::get_error_info<_expType>(*codeexcp);\
		if(p)															\
			add_errinfo(excp::get_infocode<_expType>::value, *p);		\
	}

TNERROR process_exception(const excp::ExceptionBase& exp, bool reset = true)
{
	const excp::CodeException* codeexcp = dynamic_cast<const excp::CodeException*>(&exp);

	TNERROR errorcode = TNINTERNALERROR;
	if(codeexcp)
	{
		errorcode = codeexcp->getErrorCode();
	}

	if(reset)
	{
		reset_error(errorcode, exp.why());

		if(codeexcp)
		{
			// Extract error infos
			EXTRACT_ERROR_INFO(excp::InfoCode);
			EXTRACT_ERROR_INFO(excp::CopiedEements);
			EXTRACT_ERROR_INFO(excp::BadId);
		}

#ifdef TILENET_DEBUG

		add_errinfo(TNERRI_INTERNALDEBUGDESCRIPTION, lexical_convert<string>(boost::diagnostic_information(exp)));

#endif // TILENET_DEBUG
	}

	return errorcode;
}
#undef EXTRACT_ERROR_INFO


struct TrivialExtractor
{
	template<typename Type>
	Type operator ()(const Type& v)
	{
		return v;
	}
};

template<typename Iterator, typename Type, typename Extractor>
void copy_its(Iterator begin, const Iterator end, Type* dest, size_t buflen, size_t* copied, Extractor ex)
{
	tnAssert(copied);
	tnAssert(dest);
	tnAssert(buflen);
	size_t idx = 0;

	Iterator it;
	for(it = begin; it != end && idx < buflen; ++it)
	{
		dest[idx] = ex(*it);
		++idx;
	}

	*copied = idx;

	if(it != end)
		BOOST_THROW_EXCEPTION(excp::BufferUndersizedException() << excp::CopiedEements(idx));
}

void copy_string(const string& src, wchar_t* dest, size_t buflen)
{
	tnAssert(dest);
	tnAssert(buflen);

	size_t len = std::min(src.size(), buflen-1);

	memcpy(dest, src.c_str(), len * sizeof(wchar_t));
	dest[len] = L'\0';

	if(len < src.size())
		BOOST_THROW_EXCEPTION(excp::BufferUndersizedException() << excp::CopiedEements(len+1));
}


#define AUTO_CATCH(_reset_error)																		\
	catch(const excp::ExceptionBase& e) {																		\
		return process_exception(e, _reset_error);														\
	} catch(const std::exception& e)																			\
	{ 																									\
		if(_reset_error)																				\
			reset_error(TNINTERNALERROR, lexical_convert<string>(e.what()));							\
		return TNINTERNALERROR;																			\
	} catch(const boost::exception& e)																		\
	{ 																									\
		if(_reset_error)																				\
			reset_error(TNINTERNALERROR, lexical_convert<string>(boost::diagnostic_information(e)));	\
		return TNINTERNALERROR;																			\
	} catch(...)																						\
	{ 																									\
		return TNUNKNOWN;																				\
	}

#define CHECK_RETURN(_cond, _ret)	if(!(_cond)){return (_ret);}
#define CHECK_NULL(_expr)			CHECK_RETURN(_expr, TNNULLARG)
#define CHECK_IF_ERROR()			CHECK_RETURN(LastThreadError.get(), TNNOERROR)

/// @endcond

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
 * \return TNNOERROR, if there was no error in this thread
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is an int
 * \return TNBUFFERUNDERSIZED, if buflen was smaller then the string, which should be copied!
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen )
{
	CHECK_NULL(dest);
	CHECK_NULL(buflen);
	CHECK_IF_ERROR();

	try {
		string infoString = get_error<string>(infono);
		copy_string(infoString, dest, buflen);

		return TNOK;

	} AUTO_CATCH(false);
}

/**
 * @brief Returns an error info, which is an int
 *
 * \param infono The number, identifying the certain info
 * \param dest A buffer used as destination
 * \return TNOK, if info was copied successfully
 * \return TNNOERROR, if there was no error in this thread
 * \return TNINFONOTSET, if no info with infono exists in the current error
 * \return TNWRONGINFOTYPE, if the error info is a string
 *
 * \note This function will not modify the internal (error)state, even if it fails!
 *
 **/
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest )
{
	CHECK_NULL(dest);
	CHECK_IF_ERROR();

	try {
		*dest = get_error<int>(infono);

		return TNOK;

	} AUTO_CATCH(false);
}

/**
 * @brief Fills a list with all error information codes available at the current error
 *
 * \param dest A buffer list used as destination
 * \param buflen The size of the buffer, indicating the maximum codes that will be copied
 * \param copied Variable set to the number of info codes copied by this function
 * \return TNOK, if all codes were successfully copied.
 * \return TNNOERROR, if there was no error in this thread
 * \return TNBUFFERUNDERSIZED, if buflen was smaller then the number of codes, which should be copied!
 *
 *
 **/
TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied )
{
	CHECK_NULL(dest);
	CHECK_NULL(buflen);
	CHECK_NULL(copied);
	CHECK_IF_ERROR();

	struct {
		TNERRINFO operator ()(const std::pair<TNERRINFO, ThreadErrorInfo::value_type>& p)
		{
			return p.first;
		}
	} extractor;

	try {
		copy_its(LastThreadError->infos.begin(), LastThreadError->infos.end(), dest, buflen, copied, extractor);

		return TNOK;
	} AUTO_CATCH(false);
}


/**
 * @brief Returns the last error code
 *
 * If no error occurred in this thread TNOK will be the code returned.
 *
 * \return The code of the last error occurred in the current thread.
 *
 * \note This function can not fail.
 * \note This function will not modify the internal (error)state!
 *
 **/
TNAPI TNERROR tilenet_get_last_error()
{
	if(LastThreadError.get())
		return TNOK;

	return LastThreadError->errorcode;
}




/**
 * @}
 */
