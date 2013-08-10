#pragma once
#ifndef _TILEY_IMPL_HPP
#define _TILEY_IMPL_HPP


#include <vector>
#include <string>
#include "../error.hpp"

namespace tiley {
namespace impl {

template<typename BackImpl, typename CharCaster>
class TileyImpl
{
	static BackImpl* impl;

	class CheckObj
	{
	public:
		const CheckObj& operator %(TNERROR err) const
		{
			if(err > 0)
				throw std::exception("not impl");
			return  *this;
		}
	};
	static CheckObj chk() const { return CheckObj(); }

public:
	static inline void destroy_object(TNOBJ obj)
	{
		chk() %
			impl->destroy(obj);
	}

	static inline size_t fetch_service(size_t timeout)
	{
		chk() %
			impl->fetch_service(&timeout);

		return timeout;
	}

	static inline void set_service_thread_count(size_t count)
	{
		chk() %
			impl->set_service_thread_count(count);
	}

	static inline TNSERVER create_server(const TNSVRCONFIG& init)
	{
		TNSERVER srv;

		chk() %
			impl->create_server(&srv, &init);

		return srv;
	}

	static inline TNACCEPTOR add_listen_acceptor(TNSERVER server, unsigned short port, unsigned int maxc)
	{
		TNACCEPTOR acc;

		chk() %
			impl->add_listen_acceptor(&acc, server, port, maxc);

		return acc;
	}

	static inline TNACCEPTOR add_local_acceptor(TNSERVER server)
	{
		TNACCEPTOR acc;

		chk() %
			impl->add_local_acceptor(&acc, server);

		return acc;
	}

	static inline void add_detached_local_acceptor(TNSERVER server)
	{
		chk() %
			impl->add_local_acceptor(nullptr, server);
	}

	static inline std::vector<TNEVENT> fetch_events(TNSERVER , size_t* timeout = nullptr)
	{
		std::vector<TNEVENT> evts(32);
		size_t fetched;

		chk() %
			tilenet_fetch_events(server, evts.data(), evts.size(), &fetched, nullptr);

		return evts;
	}

	static inline size_t exit(size_t timeout = 0)
	{
		chk() %
			impl->exit(&timeout);

		return timeout;
	}

	template<typename Ch>
	static inline void kick_participant(TNPARTICIPANT participant, const std::basic_string<Ch>& reason)
	{
		CharCaster caster;

		chk() %
			impl->kick(participant, caster.c_wstr(reason));
	}

	static inline void attach_layer(TNPARTICIPANT participant, TNLAYER layer)
	{
		chk() %
			impl->kick(participant, layer);
	}

	template<typename Ch>
	static inline TNLAYER create_frame(const std::basic_string<Ch>& aspectName, TNFLAG flags)
	{
		CharCaster caster;
		TNLAYER frame;


		chk() %
			impl->create_frame(&frame, caster.c_wstr(aspectName), flags);

		return frame;
	}

	static inline void tilenet_update_frame(TNLAYER frame, const std::vector<TNLAYER>& layer_list, TNVIEW** view_list)
	{
		chk() %
			impl->update_frame(frame, layer_list.data(), view_list, layer_list.size());

	}

	template<typename Ch>
	static inline TNLAYER create_tilelayer(unsigned int width, unsigned int height, TNRATIO xr, TNRATIO yr, const std::basic_string<Ch>& aspect, TNFLAG flags)
	{
		CharCaster caster;
		TNLAYER layer;

		chk() %
			impl->create_tilelayer(width, height, xr, yr, caster.c_wstr(aspect), flags);

		return layer;
	}

	static inline void update_tilelayer(TNLAYER layer, const TNTILE* tiles, const TNBOOL* toupdate)
	{
		chk() %
			impl->update_tilelayer(layer, tiles, toupdate);
	}

	template<typename Ch>
	static inline TNID stdtile(const std::basic_string<Ch>& name)
	{
		CharCaster caster;
		TNID id = 0;

		chk() %
			impl->stdtile(caster.c_wstr(name), &id);

		return id;
	}

	template<typename Ch>
	static inline TNID keyname_to_code(const std::basic_string<Ch>& name)
	{
		TNKEYCODE keycode = 0;
		CharCaster caster;

		chk() %
			impl->keycode(caster.c_wstr(name), &keycode);

		return keycode;
	}

	template<typename Ch>
	static inline std::basic_string<Ch> keycode_to_name(TNKEYCODE code)
	{
		const wchar_t* keyname = nullptr;
		CharCaster caster;

		chk() %
			impl->keyname(code, &keyname);

		return caster.str<Ch>(keyname);
	}
};

template<typename BackImpl, typename CharCaster>
BackImpl* TileyImpl<BackImpl, CharCaster>::impl = nullptr;





}}


#endif