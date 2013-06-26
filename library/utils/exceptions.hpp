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
typedef boost::error_info<Tag<TNERRI_BADID>, int>			BadId;			//!< The id which was bad
typedef boost::error_info<Tag<TNERRI_BADARG>, string>		BadArgument;	//!< Name of the bad argument
typedef boost::error_info<Tag<TNERRI_BADINDEX>, int>		BadIndex;		//!< Index of the bad field
typedef boost::error_info<Tag<TNERRI_BADSTRING>, string>	BadString;		//!< Value of the bad string 
typedef boost::error_info<struct SVFactorTag, float>		SVFactor;		//!< Indicates the severity of the violation (the larger the worse)


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

#define DEFAULT_DESCRIPTION(_excp, _desc)	template<> struct DefaultExceptionDescription<_excp>{ static const char* Description() { return _desc; } };
#define ERRCODE_DESCRIPTION(_code)			excp::DefaultExceptionDescription<excp::SpecificCodeException<_code>>::Description()


//! Base of most exceptions thrown by alacarte
struct ExceptionBase : public boost::exception, std::exception
{
	string why() const throw();
};


/**
 * @brief Exception thrown by not implemented code
 *
 **/
struct NotImplException: public ExceptionBase
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Section is not implemented!";
	}
};



struct FormatException: public ExceptionBase
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Wrong format used!";
	}
};

struct XmlException: public FormatException
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Xml has a wrong format!";
	}
};

struct XmlNotFoundException: public XmlException
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Xml entity is not found!";
	}
};

struct NetworkException: public ExceptionBase
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Network operation failed!";
	}
};

struct ProtocolException: public NetworkException
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Network communication is malformed!";
	}
};

struct DispatchException: public NetworkException
{
	virtual OVERRIDE const char* what() const throw()
	{
		return "Failed to dispatch message!";
	}
};



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
	: public CodeException
{
	virtual OVERRIDE TNERROR getErrorCode() const
	{
		return ErrCode;
	}

	virtual OVERRIDE const char* what() const throw()
	{
		return DefaultExceptionDescription<SpecificCodeException<ErrCode>>::Description();
	}

};

typedef SpecificCodeException<TNNOTSUPPORTED> NotSupportedException;
typedef SpecificCodeException<TNBUFFERUNDERSIZED> BufferUndersizedException;
typedef SpecificCodeException<TNALREADYEXISTS> AlreadyExistsException;

typedef SpecificCodeException<TNNULLARG> NullArgException;
typedef SpecificCodeException<TNINVARG> InvalidArgException;
typedef SpecificCodeException<TNBADID> BadIdException;
typedef SpecificCodeException<TNBADTYPE> BadTypeException;
typedef SpecificCodeException<TNEMPTY> EmptyStringException;

typedef SpecificCodeException<TNNOERROR> NoErrorException;
typedef SpecificCodeException<TNINFONOTSET> InfoNotSetException;
typedef SpecificCodeException<TNWRONGINFOTYPE> WrongInfoTypeException;

DEFAULT_DESCRIPTION(NotSupportedException,		"Operation is not supported by the object!");
DEFAULT_DESCRIPTION(BufferUndersizedException,	"Buffer is to small to take all informations!");
DEFAULT_DESCRIPTION(AlreadyExistsException,		"Resource already exists!");

DEFAULT_DESCRIPTION(NullArgException,			"Pointer argument is a nullptr!");
DEFAULT_DESCRIPTION(InvalidArgException,		"An argument is invalid");
DEFAULT_DESCRIPTION(BadIdException,				"Used id does not exist!");
DEFAULT_DESCRIPTION(BadTypeException,			"Object has the wrong type");
DEFAULT_DESCRIPTION(EmptyStringException,		"String is empty, but not supposed to");

DEFAULT_DESCRIPTION(NoErrorException,			"There is no last error!");
DEFAULT_DESCRIPTION(InfoNotSetException,		"Desired information is not available for the last error!");
DEFAULT_DESCRIPTION(WrongInfoTypeException,		"Desired information has another type!");


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


	//! fall back string, used when no info value is available
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