#include <iostream>
#include "tilenet.h"




int main()
{
	const char* in = "Hallo wie ügehts?";
	wchar_t out[1024];

	tilenet_convert_to_wstring(in, out, strlen(in));

	std::wcout << in;

	std::cin.get();
	return 0;
}