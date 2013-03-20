#pragma once
#ifndef _TILENET_H
#define _TILENET_H


#include <stdint.h>
#include <stddef.h>

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


/**** error codes ****/
#define TNOK				((TNERROR)0)	//!< Indicates that no error occured
#define TNUNKNOWN			((TNERROR)1)	//!< Indicates an unknown error
#define TNINTERNALERROR		((TNERROR)2)	//!< Internal error occured. This should not be returned normaly. Please submit a ticket!
#define TNBUFFERUNDERSIZED	((TNERROR)3)	//!< The given buffer was to short to take all the output

#define TNNULLARG			((TNERROR)100)	//!< Indicates, that a given argument was null but must not be null

#define TNNOERROR			((TNERROR)200)	//!< There was no last error 
#define TNINFONOTSET		((TNERROR)201)	//!< Wanted info was not set for the error
#define TNWRONGINFOTYPE		((TNERROR)202)	//!< Info has another type


/**** error info codes ****/
#define TNERRI_DESCRIPTION					((TNERRINFO)0)	//!< Gives an descritpion of the error (string)
#define TNERRI_INTERNALDEBUGDESCRIPTION		((TNERRINFO)1)	//!< If debugmode is active one can find an internal debugging description with this info (string)
#define TNERRI_INFOCODE						((TNERRINFO)2)	//!< Determinates the info code caused the error (int)
#define TNERRI_ELEMCOPIED					((TNERRINFO)3)	//!< Indicates the number of elements copied into an undersized buffer (int)


/**** server options ****/
#define TNSC_STANDALONE	0x0001
#define TNSC_NOCLIENT	0x0002
#define TNSC_CONSOLE	0x0004

/**** event types ****/
#define TNEV_CONNECT		((TNEVTYPE)1)	//!< New participant has connected
#define TNEV_DISCONNECT		((TNEVTYPE)2)	//!< Participant has disconnected

#define TNEV_KEYDOWN		((TNEVTYPE)100)	//!< Key was pressed
#define TNEV_KEYUP			((TNEVTYPE)101)	//!< Key was released

/**** tileset flags ****/
#define TNTS_GRAPHIC	0x000000
#define TNTS_STDSET		0x000 | TNTS_GRAPHIC
#define TNTS_FONTSET	0x001 | TNTS_GRAPHIC

/**** key modifier ****/
#define TNKM_SHIFT		0x001
#define TNKM_STRG		0x002
#define TNKM_ALT		0x004

/**** layer flag ****/
#define TNLF_CONSOLE	0x0004

/**** view flags ****/
#define TNVF_HFLIP		0x0001
#define TNVF_VFLIP		0x0002
#define TNVF_SIZERATIO	0x0004
#define TNVF_POSRATIO	0x0008
#define TNVF_COLORMUL	0x0010

/**** definitions ****/
typedef unsigned int	TNERROR;
typedef unsigned int	TNERRINFO;
typedef uint32_t		TNFLAG;
typedef unsigned int	TNEVTYPE;
typedef uint32_t		TNCOLOR;
typedef unsigned int	TNRATIO;
typedef unsigned int	TNID;
typedef uint32_t		TNPARTICIPANT;

#define TNSTDRATIO	1000
#define TNPARTICIPANT_TAG(_p)	(_p >> 20)
#define TNPARTICIPANT_ID(_p)	(_p & 0xFFFFF)
#define TNMAX_PARTICIPANTS		(1 << 20);

/**** objects ****/
struct TilenetObject;
typedef TilenetObject*	TNOBJ;


typedef TNOBJ TNSERVER;
typedef TNOBJ TNLAYER;
typedef TNOBJ TNTILESET;
typedef TNOBJ TNCMDSET;



/**** structs ****/

//! Config values to init the server
typedef struct TilenetServerConfig
{
	unsigned short	port;		//!< Port of the server
	unsigned int	maxc;		//!< Maximal number of connections
	const wchar_t*	name;		//!< Name of the server
	const wchar_t*	info;		//!< Info string about the server
	const wchar_t*	pkg;		//!< Standard package used for the server
	const wchar_t*	pkgi;		//!< Interface name for proper packages
	TNFLAG			options;	//!< Further options
} TNSVRCONFIG;

//! Contains information about an event
typedef struct TilenetEvent
{
	TNEVTYPE		type;			//! Type of an event
	TNPARTICIPANT	participant;	//! Participant the event coming from

	typedef union
	{
		//! In case of an keyevent this will contain information
		typedef struct
		{
			TNID	cmd;		//! An associated command
			wchar_t ch;			//! The character pressed on the keyboard
			TNFLAG	modifier;	//!	A key modifier
		} keyevent;
	} evdata;

} TNEVENT;

typedef struct TilenetView
{
	TNFLAG flags;
	TNRATIO xr;
	TNRATIO yr;
	TNRATIO wr;
	TNRATIO hr;
	unsigned int width;
	unsigned int height;
	unsigned int x;
	unsigned int y;
	TNCOLOR color;
} *TNVIEW;



/**** conversion ****/
TNAPI TNERROR tilenet_convert_to_wstring(const char* src, wchar_t* dest, size_t size);
TNAPI TNERROR tilenet_convert_to_astring(const wchar_t* src, char* dest, size_t size);

/**** error management ****/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied);
TNAPI TNERROR tilenet_get_last_error();

/**** object management *****/
TNAPI TNERROR tilenet_destroy(TNOBJ obj);
TNAPI TNERROR tilenet_clone(TNOBJ src, TNOBJ* dest);

/**** server management ****/
TNAPI TNERROR tilenet_create_server(TNSERVER* server, TNSVRCONFIG* init);
TNAPI TNERROR tilenet_start_server(TNSERVER server);
TNAPI TNERROR tilenet_fetch_events(TNSERVER server, TNEVENT* dest, size_t buflen, size_t* fetched, size_t* timeout);

/**** participant managment ****/
TNAPI TNERROR tilenet_kick(TNSERVER server, TNPARTICIPANT participant, const wchar_t* reason);
TNAPI TNERROR tilenet_attach_layer(TNSERVER server, TNPARTICIPANT participant, TNLAYER layer);
TNAPI TNERROR tilenet_attach_cmdset(TNSERVER server, TNPARTICIPANT participant, TNCMDSET set);

/**** layer: frame ****/
TNAPI TNERROR tilenet_create_frame(TNLAYER* frame, TNFLAG flags);
TNAPI TNERROR tilenet_reset_frame(TNLAYER frame);
TNAPI TNERROR tilenet_finish_frame(TNLAYER frame);
TNAPI TNERROR tilenet_frame_push(TNLAYER frame, TNLAYER* layer, TNVIEW* view);

/**** layer: tile-layer ****/
TNAPI TNERROR tilenet_create_tilelayer(TNLAYER* layer, unsigned int width, unsigned int height, TNRATIO xr, TNRATIO yr, TNFLAG flags);
TNAPI TNERROR tilenet_put_tile(TNLAYER* layer, unsigned int x, unsigned int y, void* tile);

/**** tileset ****/
TNAPI TNERROR tilenet_create_tileset(TNTILESET* set, const wchar_t* name, TNFLAG flags);
TNAPI TNERROR tilenet_register_tile(TNTILESET set, const wchar_t* name, TNID* id);

/**** commands ****/
TNAPI TNERROR tilenet_create_commandset(TNCMDSET* set, const wchar_t* name);
TNAPI TNERROR tilenet_inherit_commands(TNCMDSET base, TNCMDSET derive);
TNAPI TNERROR tilenet_add_command(TNCMDSET set, TNID id, const wchar_t* name, const wchar_t* defkey);

#ifdef __cplusplus
}
#endif

#endif
