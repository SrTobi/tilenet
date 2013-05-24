#pragma once
#ifndef _PROTOCOL_1_0_HPP
#define _PROTOCOL_1_0_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/list.hpp>

#include "settings.hpp"
#include "../protocol.hpp"
#include "protocol_tile.hpp"

#define PROTOCOL_THIS_VERSION					v1_0
#define PROTOCOL_MESSAGE(_name, _target)		namespace PROTOCOL_THIS_VERSION { namespace _target{ typedef MsgFormat<PROTOCOL_THIS_VERSION::ids::_target::_name, ::proto::versions::PROTOCOL_THIS_VERSION> _name; }} template<> struct MsgFormat<PROTOCOL_THIS_VERSION::ids::_target::_name, ::proto::versions::PROTOCOL_THIS_VERSION>
#define PROTOCOL_SERIALIZER(_ar)				friend class boost::serialization::access; template<class Archive> void serialize(Archive& _ar, const unsigned int archive_version)

namespace proto {
namespace v1_0 {
static const version_type protocol_version = PROTOCOL_MAKE_VERSION(1, 0);

namespace ids {

namespace to_srv {

	enum Ids {
		Handshake_P3_accessrequest	= 0x10

	};

}

namespace to_client {

	enum Ids {
		Handshake_P1_ProtocolVersion	= 0x70,
		Handshake_P2_ServerInformation	= 0x71,
		Handshake_P4_AcceptesGranted	= 0x72,

		LayerControl_AttachLayer		= 0x90,
		LayerControl_SendFrame			= 0x91,
		LayerControl_SendFullLayer		= 0x92,
		LayerConrtol_RemoveLayer		= 0x93
	};

}
} // ids
} // v1_0


PROTOCOL_MESSAGE(Handshake_P2_ServerInformation, to_client)
{
	string server_name;
	string server_info;
	string package_name;
	string package_interface;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & server_name & server_info & package_name & package_interface;
	}
};


PROTOCOL_MESSAGE(Handshake_P3_accessrequest, to_srv)
{
	bool accept_handshake;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & accept_handshake;
	}
};

PROTOCOL_MESSAGE(Handshake_P4_AcceptesGranted, to_client)
{
	bool access_granted;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & access_granted;
	}
};

PROTOCOL_MESSAGE(LayerControl_AttachLayer, to_client)
{
	TNID layerId;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & layerId;
	}
};

PROTOCOL_MESSAGE(LayerControl_SendFullLayer, to_client)
{
	TNID layerId;
	uint16 xratio, yratio, width, height;
	std::list<net::PTile> layerContent;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & layerId & xratio & yratio & width & height & layerContent;
	}
};

}

#undef PROTOCOL_THIS_VERSION
#undef PROTOCOL_MESSAGE
#undef PROTOCOL_SERIALIZER

#endif
