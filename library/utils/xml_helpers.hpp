#pragma once
#ifndef _XML_HELPERS_HPP
#define _XML_HELPERS_HPP


#define rxml_assert(_expr)	tnAssert(_expr)

#include "settings.hpp"
#include "utils/exceptions.hpp"

#include <rapidxml.hpp>
#include <rxml/error.hpp>
#include <rxml/locate.hpp>


namespace xml {

	typedef boost::error_info<struct _FailedNodeInfo, string> FailedNodeInfo;
	typedef boost::error_info<struct _FailedPathInfo, string> FailedPathInfo;

	struct notfound_error_generator
	{
		void operator ()(const rapidxml::xml_base<wchar_t>* node, const string& path) const
		{
			auto loc = rxml::locate(node);
			BOOST_THROW_EXCEPTION(excp::XmlNotFoundException()
									<< FailedNodeInfo(loc)
									<< FailedPathInfo(path)
									<< excp::InfoWhat(L"Failed to find '" + path + L"' in xml node '" + loc + L"'")
									);
		}
	};

	struct nomatch_error_generator
	{
		template<typename _Ch, typename _Entity>
		void operator ()(const std::basic_string<_Ch>& result, const _Entity*, const std::basic_string<_Ch>& path) const
		{
			//throw nomatch_error("sequence does not match the regex");
		}
	};

}



namespace rxml {
namespace defaults {


	template<>
	struct registry<not_found, false>
	{
		typedef xml::notfound_error_generator generator;
	};

	/*template<>
	struct registry<no_match, false>
	{
		typedef xml::nomatch_error_generator generator;
	};*/

}}





#endif