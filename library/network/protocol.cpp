#include "includes.hpp"

#include <vector>
#include "settings.hpp"


#include "protocol_deserializer.hpp"
#include "protocol_serializer.hpp"



namespace net {
namespace helper {

std::vector<bool> CreateProtocolCharMap()
{
	std::vector<bool> chars(128, true);

	chars['\0'] = false;
	chars[' '] = false;
	chars[','] = false;
	chars['{'] = false;
	chars['}'] = false;
	chars['\\'] = false;
	chars['\"'] = false;

	return chars;
}

const std::vector<bool> DirectProtocolCharMap = CreateProtocolCharMap();
	
	
bool is_direct_protocol_char(char c)
{
	if(c > 0 && c <= 127)
		return DirectProtocolCharMap[c];
	else
		return false;
}



}






}