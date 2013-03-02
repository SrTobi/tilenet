#pragma once
#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "includes.h"
#include "tilenet.h"


#ifndef __FUNCTION__
# define __FUNCTION__	"FUNCTION"
#endif


#define CONNECTION_TIMEOUT 3
#define PACKAGE_MAIN_FILE	L"package.txt"
#define TN_COLOR_WHITE tnCOLOR(255, 255, 255, 255)

typedef std::wstring nstring;
typedef sf::Uint32	nuint;
typedef sf::Int32	nint;
typedef sf::Uint8	byte;
using ::tn::uint;
using ::tn::wchar;
using ::tn::string;
using ::tn::wstring;
using ::tn::Point;
using ::tn::Rect;
using ::tn::uchar;
using ::tn::Ratio;
using ::tn::TilenetConfig;
using ::boost::system::error_code;
using ::boost::shared_ptr;
using ::boost::weak_ptr;
using ::boost::make_shared;

namespace fs {
	using namespace boost::filesystem;
}

namespace asio {
	using namespace boost::asio;
	using namespace boost::asio::ip;
}

namespace ptime {
	using namespace boost::posix_time;
	typedef boost::posix_time::time_duration duration;
}

typedef boost::shared_ptr<sf::TcpSocket>		SocketPtr;
typedef boost::shared_ptr<asio::tcp::socket>	socket_ptr;
typedef boost::lock_guard<boost::mutex>			sync_guard;

inline sf::Packet& operator << (sf::Packet& pack, const tn::basic_vector2<int>& v)
{
	pack << (sf::Int32)v.x << (sf::Int32)v.y;
	return pack;
}
inline sf::Packet& operator >> (sf::Packet& pack, tn::basic_vector2<int>& v)
{
	sf::Int32 i;
	pack >> i;
	v.x = i;
	pack >> i;
	v.y = i;

	return pack;
}


namespace tool {

template<typename T>
bool between(T value, T lowerLimit, T upperLimit)
{
	return (value >= lowerLimit && value <= upperLimit);
}


struct PointComparator {
  bool operator() (const Point& p1, const Point& p2)
  {
	  if(p1.y == p2.y)
		  return p1.x < p2.x;
	  return p1.y < p2.y;
  }
};

template<typename MUTEX>
class noguard
{
private:

	explicit noguard(noguard&);
	noguard& operator=(noguard&);
public:
	explicit noguard(MUTEX& m_)
	{
	}
	noguard(MUTEX& m_, boost::adopt_lock_t)
	{}
	~noguard()
	{
	}
};

}

namespace logg {
enum LogLvls
{
	Error		= 0x1,
	Exception	= 0x2,
	Info		= 0x4,
	Warning		= 0x8,
	Network		= 0x10,
	Construct	= 0x20,
	TileReg		= 0x40,
	TileReqest	= 0x80,
	Default		= Error | Exception | Info
};

extern std::wostream	_tnServerLogOutput;
extern std::wostream	_tnClientLogOutput;
extern std::wostream	_tnServiceLogOutput;
extern uint				_tnCurrentDebugLevel;
extern boost::mutex		LogMutex;


#define _TN_TO_UNICODE(str) L## str
#define _uni(str) _TN_TO_UNICODE(str)


#define SLOG(_dlvl, _msg)	{boost::mutex::scoped_lock lock(logg::LogMutex); if(logg:: _dlvl & logg::_tnCurrentDebugLevel){ logg::_tnServerLogOutput << L"[" _uni(#_dlvl) L"]Server:\t" << _msg << std::endl; }}
#define CLOG(_dlvl, _msg)	{boost::mutex::scoped_lock lock(logg::LogMutex); if(logg:: _dlvl & logg::_tnCurrentDebugLevel){ logg::_tnClientLogOutput << L"[" _uni(#_dlvl) L"]Client:\t" << _msg << std::endl; }}
#define SVLOG(_dlvl, _msg)	{boost::mutex::scoped_lock lock(logg::LogMutex); if(logg:: _dlvl & logg::_tnCurrentDebugLevel){ logg::_tnServiceLogOutput << L"[" _uni(#_dlvl) L"]NetService:\t" << _msg << std::endl; }}
#define STHROW(_msg)		{const wchar* msg = _msg; SLOG(Exception, _msg); throw tn::TnException(_msg); }
#define NOT_IMPL()			{STHROW(L"This part is not implemented![" _uni(__FUNCTION__)   L"]");}
}




namespace version {
	static const wstring VersionNames[] = {
		L"no version!",
		L"tilepower"
	};

	static const uint CurrentVersion = 1;
}


// Standartincludes
#include "utils/enable_shared_for_derived.h"
#include "utils/syncronized.h"

using utils::Syncronized;
using utils::syncronize;
#endif
