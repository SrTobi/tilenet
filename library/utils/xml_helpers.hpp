#pragma once
#ifndef _XML_HELPERS_HPP
#define _XML_HELPERS_HPP


#include <rapidxml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "settings.hpp"

namespace rxml = rapidxml;


namespace xml {



template<typename Ch, typename Str>
rxml::xml_node<Ch>* get_node(rxml::xml_node<Ch>* node, Str path)
{
	std::basic_string<Ch> name;
	std::basic_istringstream<Ch> iss(path);

	while(std::getline(iss, name, Ch('/')))
	{
		node = node->first_node(name.c_str(), name.size());

		if(!node)
			return nullptr;
	}

	return node;
}


template<typename Ty, typename Ch>
Ty get_attr(rxml::xml_node<Ch>* node, const std::basic_string<Ch>& path, const std::basic_string<Ch>& attr)
{
	node = get_node(node, path);

	if(!node)
		NOT_IMPLEMENTED();

	auto* a = node->first_attribute(attr.c_str(), attr.size());

	if(!a)
		NOT_IMPLEMENTED();

	return boost::lexical_cast<Ty>(a->value(), a->value_size());
}


template<typename Ty, typename Ch>
Ty get_attr(rxml::xml_node<Ch>* node, const std::basic_string<Ch>& path, const std::basic_string<Ch>& attr, const Ty& def)
{
	node = get_node(node, path);

	if(!node)
		return def;

	auto* a = node->first_attribute(attr.c_str(), attr.size());

	if(!a)
		return def;

	return boost::lexical_cast<Ty>(a->value(), a->value_size());
}




}





#endif