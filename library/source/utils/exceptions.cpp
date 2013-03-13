#include "includes.hpp"

#include "settings.hpp"

namespace excp {


	string ExceptionBase::why() const throw()
	{
		if(string const* info = boost::get_error_info<InfoWhat>(*this))
			return *info;
		return lexical_convert<string>(std::exception::what());
	}

};