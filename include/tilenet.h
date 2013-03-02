#pragma once
#ifndef _TILENET_H
#define _TILENET_H


#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef TILENET_AS_LIB
#	define TNAPI
#else
#	ifdef WIN32
#		ifdef TILENET_EXPORTS
#			define TNAPI __declspec(dllexport)
#		else
#			define TNAPI __declspec(dllimport)
#		endif
#	else
#		define TNAPI __attribute__ ((__visibility__ ("default")))
#	endif
#endif

#define TNERRI_DESCRIPTION	1

typedef unsigned int TNERROR;
typedef unsigned int TNERRINFO;
typedef unsigned int TNIDTYPE;

typedef TNIDTYPE TNSERVERID;

TNAPI TNERROR tilenet_convert_to_wstring(const char* src, wchar_t* dest);
TNAPI TNERROR tilenet_convert_to_astring(const wchar_t* src, char* dest);

TNAPI TNERROR tilenet_get_error_string(TNERROR err, TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERROR err, TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERROR err, TNERRINFO* dest, size_t buflen, size_t* cpylen);

TNAPI TNERROR tilenet_create_server(TNSERVERID* newid);
TNAPI TNERROR tilenet_destroy_server(TNSERVERID srvid);






#ifdef __cplusplus
}
#endif

#endif