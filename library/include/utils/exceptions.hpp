#pragma once
#ifndef _EXCEPTIONS_HPP
#define _EXCEPTIONS_HPP

/// @cond DEV

#include <boost/exception/all.hpp>

#include "settings.hpp"


namespace excp {


template<TNERRINFO I>
struct Tag {};


typedef boost::error_info<Tag<TNERRI_DESCRIPTION>, string>	InfoWhat;		//!< Short describtion of the exception
typedef boost::error_info<Tag<TNERRI_INFOCODE>, int>		InfoCode;		//!< error info code that raised the exception
typedef boost::error_info<Tag<TNERRI_ELEMCOPIED>, int>		CopiedEements;	//!< Number of elements already copied by an operation
typedef boost::error_info<Tag<TNERRI_BADID>, int>			BadId;

template<typename ErrorInfo>
struct get_infocode
{
	static_assert(sizeof(ErrorInfo) != sizeof(ErrorInfo), "ErrorInfo is no boost::error");
};

template<TNERRINFO I, typename Param>
struct get_infocode<boost::error_info<Tag<I>, Param> >
	: public std::integral_constant<TNERRINFO, I>
{
};


template<typename Exception>
struct DefaultExceptionDescription
{
	static_assert(sizeof(Exception) != sizeof(Exception), "'Exception' has no default description!");
};

#define DEFAULT_DESCRIPTION(_excp, _desc) template<> struct DefaultExceptionDescription<_excp>{ static const char* Description() { return _desc; } };

//! Base of most exceptions thrown by alacarte
struct ExceptionBase : public boost::exception, std::exception
{
	string why() const throw();
};


/**
 * @brief Exception thrown by not implemented code
 *
 **/
struct NotImplException: public ExceptionBase {};

/**
 * @brief Exception having a certain error code which could be used directly with the c api
 *
 **/
struct CodeException
	: public ExceptionBase
{
	virtual TNERROR getErrorCode() const = 0;
};

template<TNERROR ErrCode>
struct SpecificCodeException
	: public ExceptionBase
{
	virtual override TNERROR getErrorCode() const
	{
		return ErrCode;
	}

	virtual override const char* what() const throw()
	{
		return DefaultExceptionDescription<SpecificCodeException<ErrCode>>::Description();
	}

};


typedef SpecificCodeException<TNINFONOTSET> InfoNotSetException;
typedef SpecificCodeException<TNWRONGINFOTYPE> WrongInfoTypeException;
typedef SpecificCodeException<TNBUFFERUNDERSIZED> BufferUndersizedException;
typedef SpecificCodeException<TNNOERROR> NoErrorException;
typedef SpecificCodeException<TNBADID> BadIdException;
typedef SpecificCodeException<TNNOTSUPPORTED> NotSupportedException;

DEFAULT_DESCRIPTION(InfoNotSetException,		"Desired information is not available for the last error!");
DEFAULT_DESCRIPTION(WrongInfoTypeException,		"Desired information has another type!");
DEFAULT_DESCRIPTION(BufferUndersizedException,	"Buffer is to small to take all informations!");
DEFAULT_DESCRIPTION(NoErrorException,			"There is no last error!");
DEFAULT_DESCRIPTION(BadIdException,				"Used id does not exist!");
DEFAULT_DESCRIPTION(NotSupportedException,		"Operation is not supported by the object!");


/**
 * @brief Utility tool for output a certain error info.
 *
 **/
template<typename Info>
struct ErrorOut
{
	typedef typename boost::exception_detail::get_error_info_return_type<boost::exception,typename Info::value_type>::type value_type;

	/**
	 * @brief Creates the ErrorOut object with an exception
	 *
	 * \param e The exception, that maybe contains the information
	 * \param falbackInfo A fallback info used when die info is not available on the exception
	 **/
	ErrorOut(boost::exception& e, const string& fallbackInfo = "'unknown'")
		: fallbackInfo(fallbackInfo)
		, value(boost::get_error_info<Info>(e))
	{
	}

	/**
	 * @brief Outputs the info of the given exception of the fallback message
	 *
	 * \return info or fallback message
	 **/
	operator string() const
	{
		std::istringstream is;
		is << *this;
		return is.str();
	}


	//! Fallback string, used when no info value is available
	string fallbackInfo;

	//! info of the exception
	value_type value;
};

template<typename Stream, typename Info>
inline std::basic_ostream<Stream>& operator <<(std::basic_ostream<Stream>& os, const ErrorOut<Info>& info)
{
	if(info.value)
	{
		os << *info.value;
	} else {
		os << info.fallbackInfo;
	}

	return os;
}


}


/// @endcond

#endif