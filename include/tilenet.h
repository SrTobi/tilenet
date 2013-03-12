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

#define TNOK				((TNERROR)0)	//!< Indicates that no error occured
#define TNUNKNOWN			((TNERROR)1)	//!< Indicates an unknown error
#define TNINTERNALASSERTION	((TNERROR)2)	//!< Internal assertion occured. This should not be returned normaly. Please submit a ticket!
#define TNBUFFERUNDERSIZED	((TNERROR)3)	//!< The given buffer was to short to take all the output

#define TNNULLARG			((TNERROR)100)	//!< Indicates, that a given argument was null but must not be null

#define TNINFONOTSET		((TNERROR)200)	//!< Wanted info was not set for the error
#define TNWRONGINFOTYPE		((TNERROR)201)	//!< Info has another type

#define TNERRI_DESCRIPTION	0	//!< Gives an descritpion of the error (string)
#define TNERRI_INFOCODE		1	//!< Determinates the info code caused the error (int)
#define TNERRI_ELEMCOPIED	2	//!< Indicates the number of elements copied into an undersized buffer (int)


struct TilenetObject;

typedef unsigned int TNERROR;
typedef unsigned int TNERRINFO;
typedef TilenetObject* TNOBJ;

typedef TNOBJ TNSERVER;

TNAPI TNERROR tilenet_convert_to_wstring(const char* src, wchar_t* dest, size_t size);
TNAPI TNERROR tilenet_convert_to_astring(const wchar_t* src, char* dest, size_t size);

/**** error management ****/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied);

/**** id management *****/
TNAPI TNERROR tilenet_addref(TNOBJ obj);
TNAPI TNERROR tilenet_release(TNOBJ obj);
TNAPI TNERROR tilenet_weak(TNOBJ obj, TNOBJ* weak);
TNAPI TNERROR tilenet_unweak(TNOBJ weak, TNOBJ* obj);
TNAPI TNERROR tilenet_get_refcount(TNOBJ obj, size_t* refs);
TNAPI TNERROR tilenet_get_weakcount(TNOBJ obj, size_t* refs);
TNAPI TNERROR tilenet_destroy(TNOBJ obj, size_t* refs);

/**** server management ****/
TNAPI TNERROR tilenet_get_object_server(TNOBJ obj, TNSERVER* srvObj);
TNAPI TNERROR tilenet_create_server(TNSERVER* newObj);
TNAPI TNERROR tilenet_destroy_server(TNSERVER srvObj);
TNAPI TNERROR tilenet_get_server_list(TNSERVER* dest, size_t buflen, size_t* copied);





#ifdef __cplusplus
}
#endif

#endif