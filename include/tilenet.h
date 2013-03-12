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


struct TilenetObject;

typedef unsigned int TNERROR;
typedef unsigned int TNERRINFO;
typedef TilenetObject* TNOBJ;

typedef TNOBJ TNSERVER;

TNAPI TNERROR tilenet_convert_to_wstring(const char* src, wchar_t* dest, size_t size);
TNAPI TNERROR tilenet_convert_to_astring(const wchar_t* src, char* dest, size_t size);

/**** error management ****/
TNAPI TNERROR tilenet_get_error_string(TNERROR err, TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERROR err, TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERROR err, TNERRINFO* dest, size_t buflen, size_t* copied);

/**** id management *****/
TNAPI TNERROR tilenet_addref(TNOBJ obj);
TNAPI TNERROR tilenet_release(TNOBJ obj);
TNAPI TNERROR tilenet_weak(TNOBJ obj, TNOBJ* weak);
TNAPI TNERROR tilenet_unweak(TNOBJ weak, TNOBJ* obj);
TNAPI TNERROR tilenet_refcount(TNOBJ obj, size_t* refs);
TNAPI TNERROR tilenet_weakcount(TNOBJ obj, size_t* refs);
TNAPI TNERROR tilenet_destroy(TNOBJ id);
TNAPI TNERROR tilenet_gc(size_t* collected);


/**** server management ****/
TNAPI TNERROR tilenet_get_object_server(TNOBJ obj, TNSERVER* srvObj);
TNAPI TNERROR tilenet_create_server(TNSERVER* newObj);
TNAPI TNERROR tilenet_destroy_server(TNSERVER srvObj);
TNAPI TNERROR tilenet_get_server_list(TNSERVER* dest, size_t buflen, size_t* copied);





#ifdef __cplusplus
}
#endif

#endif