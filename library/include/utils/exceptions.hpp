#pragma once
#ifndef _EXCEPTIONS_HPP
#define _EXCEPTIONS_HPP

#include <boost/exception/all.hpp>

#include "settings.hpp"


namespace excp {


template<TNERRINFO I>
struct Tag {};


typedef boost::error_info<Tag<TNERRI_DESCRIPTION>, string>	InfoWhat;
typedef boost::error_info<Tag<TNERRI_INFOCODE>, int>		InfoCode;
typedef boost::error_info<Tag<TNERRI_ELEMCOPIED>, int>		CopiedEements;

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



//! Base of most exceptions thrown by alacarte
struct ExceptionBase : public boost::exception, std::exception
{
	string why() const throw();
};


struct NotImplException: public ExceptionBase {};


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
};


typedef SpecificCodeException<TNINFONOTSET> InfoNotSetException;
typedef SpecificCodeException<TNWRONGINFOTYPE> WrongInfoTypeException;
typedef SpecificCodeException<TNBUFFERUNDERSIZED> BufferUndersizedException;
typedef SpecificCodeException<TNNOERROR> NoErrorException;

template<typename Info>
struct ErrorOut
{
	typedef typename boost::exception_detail::get_error_info_return_type<boost::exception,typename Info::value_type>::type value_type;


	ErrorOut(boost::exception& e, const string& fallbackInfo = "'unknown'")
		: fallbackInfo(fallbackInfo)
		, value(boost::get_error_info<Info>(e))
	{
	}

	operator string() const
	{
		std::istringstream is;
		is << *this;
		return is.str();
	}


	string fallbackInfo;
	value_type value;
};

template<typename Stream, typename Info>
std::basic_ostream<Stream>& operator <<(std::basic_ostream<Stream>& os, const ErrorOut<Info>& info)
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


#endif