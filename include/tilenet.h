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

#define TNOK				((TNERROR)0)
#define TNFAILED			((TNERROR)1)

#define TNERRI_DESCRIPTION	0

typedef unsigned int TNERROR;
typedef unsigned int TNERRINFO;
typedef unsigned int TNIDTYPE;

typedef TNIDTYPE TNWEAKID;
typedef TNIDTYPE TNSERVERID;

TNAPI TNERROR tilenet_convert_to_wstring(const char* src, wchar_t* dest, size_t size);
TNAPI TNERROR tilenet_convert_to_astring(const wchar_t* src, char* dest, size_t size);

/**** error management ****/
TNAPI TNERROR tilenet_get_error_string(TNERROR err, TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERROR err, TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERROR err, TNERRINFO* dest, size_t buflen, size_t* copyed);

/**** id management *****/
TNAPI TNERROR tilenet_addref(TNIDTYPE id);
TNAPI TNERROR tilenet_release(TNIDTYPE id);
TNAPI TNERROR tilenet_weak(TNIDTYPE id, TNIDTYPE* weak);
TNAPI TNERROR tilenet_unweak(TNIDTYPE weak, TNIDTYPE* id);
TNAPI TNERROR tilenet_refcount(TNIDTYPE id, size_t* refs);
TNAPI TNERROR tilenet_weakcount(TNIDTYPE id, size_t* refs);
TNAPI TNERROR tilenet_gc(TNSERVERID id, size_t* collected);

/**** server management ****/
TNAPI TNERROR tilenet_create_server(TNSERVERID* newid);
TNAPI TNERROR tilenet_destroy_server(TNSERVERID srvid);
TNAPI TNERROR tilenet_get_server_list(TNSERVERID* dest, size_t buflen, size_t* copyed);





#ifdef __cplusplus
}
#endif

#endif