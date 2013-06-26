#pragma once
#ifndef _TILENET_H
#define _TILENET_H


#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @file */

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

/**
 * @defgroup ERRORS Errors
 * @{
 */

/** @name Error codes
 *	@{
 *
 *	Codes returned by tilenet functions indicating errors, fails or success. See @ref error_handling for more informations.
 */

#define TNOK				((TNERROR)0)	//!< Indicates that no error occured
#define TNUNKNOWN			((TNERROR)1)	//!< Indicates an unknown error
#define TNINTERNALERROR		((TNERROR)2)	//!< Internal error occured. This should not be returned normally. Please submit a ticket!
#define TNNOTSUPPORTED		((TNERROR)3)	//!< An operation is not supported in the current version or compilation mode or by the used object
#define TNBUFFERUNDERSIZED	((TNERROR)4)	//!< The given buffer was to short to take all the output

#define TNNULLARG			((TNERROR)100)	//!< Indicates, that a given argument was null but must not be null
#define TNBADID				((TNERROR)101)	//!< Indicates, that a given id does not exist
#define TNBADTYPE			((TNERROR)103)	//!< The object had the wrong type
#define TNEMPTY				((TNERROR)104)	//!< An argument was empty (e.g. a string)

#define TNNOERROR			((TNERROR)200)	//!< There was no last error 
#define TNINFONOTSET		((TNERROR)201)	//!< Wanted info was not set for the error
#define TNWRONGINFOTYPE		((TNERROR)202)	//!< Info has another type
	
/**** fail codes ****/
#define TNTIMEOUT			((TNERROR)-1)	//!< The operation returned because of a timeout (this code will not cause a change in the internal error state!)

///@} (Error codes)
	
/** @name [ERR_INFO_CODES] Error information
 *	@{
 *	
 *	Codes to get informations about the last error. See @ref error_infos for more informations.
 */

/**** error info codes ****/
#define TNERRI_DESCRIPTION					((TNERRINFO)0)	//!< Gives an descritpion of the error (string)
#define TNERRI_INTERNALDEBUGDESCRIPTION		((TNERRINFO)1)	//!< If debugmode is active one can find an internal debugging description with this info (string)
#define TNERRI_INFOCODE						((TNERRINFO)2)	//!< Determinates the info code caused the error (int)
#define TNERRI_ELEMCOPIED					((TNERRINFO)3)	//!< Indicates the number of elements copied into an undersized buffer (int)
#define TNERRI_BADID						((TNERRINFO)4)	//!< Determinates the id that caused the error (int)
#define TNERRI_BADARG						((TNERRINFO)5)	//!< Returns the name of the argument, which caused the error

//@} (Error information)

//@} (ERRORS)

/**** server options ****/
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
#define TNKM_CONTROL	0x002
#define TNKM_ALT		0x004
#define TNKM_ALTGR		0x008

/**** layer flag ****/
#define TNLF_CONSOLE	0x0004

/**** view flags ****/
#define TNVF_HFLIP		0x0001
#define TNVF_VFLIP		0x0002
#define TNVF_SIZERATIO	0x0004
#define TNVF_POSRATIO	0x0008
#define TNVF_COLORMUL	0x0010

/**** char modifier ****/
#define TNCT_BOLD		0x0001000
#define TNCT_ITALIC		0x0002000
#define TNCT_UNDERLINE	0x0004000

/**** definitions ****/
typedef int				TNERROR;
typedef unsigned int	TNERRINFO;
typedef unsigned char	TNBOOL;
typedef uint32_t		TNFLAG;
typedef unsigned int	TNEVTYPE;
typedef uint32_t		TNCOLOR;
typedef float			TNRATIO;
typedef unsigned int	TNID;
typedef uint32_t		TNPARTICIPANT;
typedef unsigned int	TNKEYCODE;

#define TNSTDRATIO						(1.0f)
#define TN_ID_BITLENGTH					28
#define TN_TAG_BITLENGTH				((sizeof(TNID)*8) - TN_ID_BITLENGTH)
#define TNEXTRACTTAG(_p)				(_p >> TN_ID_BITLENGTH)
#define TNEXTRACTID(_p)					(_p & TNMAX_ID)
#define TNMAX_ID						((1 << TN_ID_BITLENGTH) - 1)
#define TNMAX_IDTAG						((1 << TN_TAG_BITLENGTH) - 1)
#define TNMAX_PARTICIPANTS				(TNMAX_ID + 1);
#define TNGET_ALPHA(_color)				(((_color) >> 24) & 0xFF)
#define TNGET_RED(_color)				(((_color) >> 16) & 0xFF)
#define TNGET_GREEN(_color)				(((_color) >> 8) & 0xFF)
#define TNGET_BLUE(_color)				((_color) & 0xFF)
#define TNMAKE_ALPHA(_color)			(((_color) & 0xFF) << 24)
#define TNMAKE_RED(_color)				(((_color) & 0xFF) << 16)
#define TNMAKE_GREEN(_color)			(((_color) & 0xFF) << 8)
#define TNMAKE_BLUE(_color)				((_color) & 0xFF)
#define TNMAKE_COLOR(_a, _r, _g, _b)	(TNMAKE_ALPHA(_a) | TNMAKE_RED(_r) | TNMAKE_GREEN(_g) | TNMAKE_BLUE(_b))

/**** objects ****/
struct TilenetObject;
typedef TilenetObject*	TNOBJ;


typedef TNOBJ TNSERVER;
typedef TNOBJ TNLAYER;
typedef TNOBJ TNTILESET;
typedef TNOBJ TNCMDSET;
typedef TNOBJ TNACCEPTOR;


/**** structs ****/

//! Config values to init the server
typedef struct TilenetServerConfig
{
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

	typedef struct
	{
		TNKEYCODE	key;		//! An associated keycode
		TNFLAG		modifier;	//!	A key modifier
	} keyevent_type;

	union
	{
		//! In case of an keyevent this will contain information
		keyevent_type keyevent;
	} data;

} TNEVENT;

typedef enum TileTypes
{
	TN_NULL_TILE = 0,
	TN_STD_TILE = 1,
	TN_CHAR_TILE = 2
} TNTILIETYPE;

typedef struct TilenetTile
{
	TNTILIETYPE type;

	typedef struct {

	} nullset_type;

	typedef struct {
		TNID	id;
		TNFLAG	modifier;
		TNCOLOR color;
	} stddata_type;

	typedef struct {
		wchar_t ch;
		TNFLAG	modifier;
		TNCOLOR color;
	} chardata_type;

	union
	{
		stddata_type stdset;
		chardata_type charset;
	} data;

} TNTILE;


typedef struct TilenetView
{
	TNFLAG flags;

	TNRATIO outter_xr;
	TNRATIO outter_yr;
	TNRATIO outter_wr;
	TNRATIO outter_hr;
	unsigned int outter_x;
	unsigned int outter_y;
	unsigned int outter_w;
	unsigned int outter_h;

	TNRATIO inner_xr;
	TNRATIO inner_yr;
	TNRATIO inner_wr;
	TNRATIO inner_hr;
	unsigned int inner_x;
	unsigned int inner_y;
	unsigned int inner_w;
	unsigned int inner_h;

	TNCOLOR color;
} TNVIEW;



/**** error management ****/
TNAPI TNERROR tilenet_get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen);
TNAPI TNERROR tilenet_get_error_int(TNERRINFO infono, int* dest);
TNAPI TNERROR tilenet_get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied);
TNAPI TNERROR tilenet_get_last_error();

/**** object management *****/
TNAPI TNERROR tilenet_destroy(TNOBJ obj);

/**** service managment ****/
TNAPI TNERROR tilenet_fetch_service(size_t* timeout);
TNAPI TNERROR tilenet_set_service_thread_count(size_t count);

/**** server management ****/
TNAPI TNERROR tilenet_create_server(TNSERVER* server, const TNSVRCONFIG* init);
TNAPI TNERROR tilenet_add_listen_acceptor(TNSERVER server, unsigned short port, unsigned int maxc);
TNAPI TNERROR tilenet_add_local_acceptor(TNACCEPTOR* acceptor, TNSERVER server);
TNAPI TNERROR tilenet_fetch_events(TNSERVER server, TNEVENT* dest, size_t buflen, size_t* fetched, size_t* timeout);
TNAPI TNERROR tilenet_exit(size_t* timeout);

/**** participant managment ****/
TNAPI TNERROR tilenet_kick(TNPARTICIPANT participant, const wchar_t* reason);
TNAPI TNERROR tilenet_attach_layer(TNPARTICIPANT participant, TNLAYER layer);

/**** layer: frame ****/
TNAPI TNERROR tilenet_create_frame(TNLAYER* frame, TNFLAG flags);
TNAPI TNERROR tilenet_update_frame(TNLAYER frame, TNLAYER* layer_list, TNVIEW** view_list, size_t size);

/**** layer: tile-layer ****/
TNAPI TNERROR tilenet_create_tilelayer(TNLAYER* layer, unsigned int width, unsigned int height, TNRATIO xr, TNRATIO yr, TNFLAG flags);
TNAPI TNERROR tilenet_update_tilelayer(TNLAYER layer, const TNTILE* tiles, const TNBOOL* toupdate);

/**** tileset ****/
TNAPI TNERROR tilenet_stdtile(const wchar_t* name, TNID* id);

/**** key managment ****/
TNAPI TNERROR tilenet_keycode(const wchar_t* name, TNKEYCODE* code);
TNAPI TNERROR tilenet_keyname(TNKEYCODE code, const wchar_t** name);

#ifdef __cplusplus
}
#endif

#endif
