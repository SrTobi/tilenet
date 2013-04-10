#pragma once
#ifndef _PROTOCOL_HPP
#define _PROTOCOL_HPP


#include "settings.hpp"
#include "message.hpp"


#define PROTOCOL_MESSAGE(_name, _target)		namespace _target{ typedef MsgFormat<ids::_target::_name> _name; } template<> struct MsgFormat<ids::_target::_name>
#define PROTOCOL_SERIALIZER(_ar)				friend class boost::serialization::access; template<class Archive> void serialize(Archive& _ar, const unsigned int archive_version)
#define PROTOCOL_MAKE_VERSION(_major, _minor)	version_type(version_type(_major) << 16 | version_type(_minor))


namespace proto {

static const version_type protocol_version = PROTOCOL_MAKE_VERSION(1, 0);

template<msgid_type Id>
struct MsgFormat
{
};

namespace ids {

namespace to_srv {

	enum Ids {


	};

}

namespace to_client {

	enum Ids {
		Handshake_P1_ProtocolVersion = 100



	};

}

}

PROTOCOL_MESSAGE(Handshake_P1_ProtocolVersion, to_client)
{
	version_type version;
	PROTOCOL_SERIALIZER(ar)
	{
		ar & version;
	}
};






}






#endif