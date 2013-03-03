#include <iostream>
#include "tilenet.h"




int main()
{
	const wchar_t* in = L"Hallo wie übergehts?";
	char out[1024];

	tilenet_convert_to_astring(in, out, std::char_traits<wchar_t>::length(in)+1);

	std::cout << out;

	std::cin.get();
	return 0;
}