#pragma once
#ifndef _PROTOCOL_HPP
#define _PROTOCOL_HPP


#include "settings.hpp"
//#include "message.hpp"


#define PROTOCOL_MAKE_VERSION(_major, _minor)	version_type(version_type(_major) << 16 | version_type(_minor))
#define PROTOCOL_SET_CURRENT_VERSION(_v) namespace proto{namespace _v {} namespace curv = _v; }


namespace proto {

namespace versions {

	enum Version
	{
		NoVersion,
		v1_0
	};

}

template<msgid_type Id, versions::Version V>
struct MsgFormat;


template<>
struct MsgFormat<0x01, versions::NoVersion>
{
	version_type version;
	template<typename Archive>
	void serialize(Archive& ar, unsigned int archive_version)
	{
		ar & version;
	}
};
typedef MsgFormat<1, versions::NoVersion> ComInitializing_ProtocolVersion;

template<msgid_type Id, versions::Version V>
struct MsgFormat
{
};
}


#include "v1.0/protocol_v1_0.hpp"

PROTOCOL_SET_CURRENT_VERSION(v1_0);

#endif
