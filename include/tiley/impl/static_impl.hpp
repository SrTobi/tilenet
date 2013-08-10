#pragma once
#ifndef _TILEY_STATIC_IMPL
#define _TILEY_STATIC_IMPL

#include "tilenet.h"

namespace tiley {
namespace impl {



class StaticImpl
{
public:
	/**** error management ****/
	inline TNERROR get_error_string(TNERRINFO infono, wchar_t* dest, size_t buflen)
	{
		return tilenet_get_error_string(infono, dest, buflen);
	}

	inline TNERROR get_error_int(TNERRINFO infono, int* dest)
	{
		return tilenet_get_error_int(infono, dest);
	}

	inline TNERROR get_info_list(TNERRINFO* dest, size_t buflen, size_t* copied)
	{
		return tilenet_get_info_list(dest, buflen, copied);
	}

	inline TNERROR get_last_error()
	{
		return tilenet_get_last_error();
	}


	/**** object management *****/
	inline TNERROR destroy(TNOBJ obj)
	{
		return tilenet_destroy(obj);
	}


	/**** service managment ****/
	inline TNERROR fetch_service(size_t* timeout)
	{
		return tilenet_fetch_service(timeout);
	}

	inline TNERROR set_service_thread_count(size_t count)
	{
		return tilenet_set_service_thread_count(count);
	}


	/**** server management ****/
	inline TNERROR create_server(TNSERVER* server, const TNSVRCONFIG* init)
	{
		return tilenet_create_server(server, init);
	}

	inline TNERROR add_listen_acceptor(TNACCEPTOR* acceptor, TNSERVER server, unsigned short port, unsigned int maxc)
	{
		return tilenet_add_listen_acceptor(acceptor, server, port, maxc);
	}

	inline TNERROR add_local_acceptor(TNACCEPTOR* acceptor, TNSERVER server)
	{
		return tilenet_add_local_acceptor(acceptor, server);
	}

	inline TNERROR fetch_events(TNSERVER server, TNEVENT* dest, size_t buflen, size_t* fetched, size_t* timeout)
	{
		return tilenet_fetch_events(server, dest, buflen, fetched, timeout);
	}

	inline TNERROR exit(size_t* timeout)
	{
		return tilenet_exit(timeout);
	}


	/**** participant managment ****/
	inline TNERROR kick(TNPARTICIPANT participant, const wchar_t* reason)
	{
		return tilenet_kick(participant, reason);
	}

	inline TNERROR attach_layer(TNPARTICIPANT participant, TNLAYER layer)
	{
		return tilenet_attach_layer(participant, layer);
	}


	/**** layer: frame ****/
	inline TNERROR create_frame(TNLAYER* frame, const wchar_t* aspectName, TNFLAG flags)
	{
		return tilenet_create_frame(frame, aspectName, flags);
	}

	inline TNERROR update_frame(TNLAYER frame, TNLAYER* layer_list, TNVIEW** view_list, size_t size)
	{
		return tilenet_update_frame(frame, layer_list, view_list, size);
	}


	/**** layer: tile-layer ****/
	inline TNERROR create_tilelayer(TNLAYER* layer, unsigned int width, unsigned int height, TNRATIO xr, TNRATIO yr, const wchar_t* aspect, TNFLAG flags)
	{
		return tilenet_create_tilelayer(layer, width, height, xr, yr, aspect, flags);
	}

	inline TNERROR update_tilelayer(TNLAYER layer, const TNTILE* tiles, const TNBOOL* toupdate)
	{
		return tilenet_update_tilelayer(layer, tiles, toupdate);
	}


	/**** tileset ****/
	inline TNERROR stdtile(const wchar_t* name, TNID* id)
	{
		return tilenet_stdtile(name, id);
	}


	/**** key managment ****/
	inline TNERROR keycode(const wchar_t* name, TNKEYCODE* code)
	{
		return tilenet_keycode(name, code);
	}

	inline TNERROR keyname(TNKEYCODE code, const wchar_t** name)
	{
		return tilenet_keyname(code, name);
	}

};






}}







#endif
