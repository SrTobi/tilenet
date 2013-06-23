#pragma once
#ifndef _PROTOCOL_1_0_HPP
#define _PROTOCOL_1_0_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

#include <vector>

#include "settings.hpp"
#include "../protocol.hpp"
#include "protocol_tile.hpp"
#include "key_mapping_v1_0.hpp"
#include "protocol_view_v1_0.hpp"
#include "utils/boost_unique_ptr_serialization.hpp"

#define PROTOCOL_THIS_VERSION					v1_0
#define PROTOCOL_MESSAGE(_name, _target)		namespace PROTOCOL_THIS_VERSION { namespace _target{ typedef MsgFormat<PROTOCOL_THIS_VERSION::ids::_target::_name, ::proto::versions::PROTOCOL_THIS_VERSION> _name; }} template<> struct MsgFormat<PROTOCOL_THIS_VERSION::ids::_target::_name, ::proto::versions::PROTOCOL_THIS_VERSION>
#define PROTOCOL_SERIALIZER(_ar)				friend class boost::serialization::access; template<class Archive> void serialize(Archive& _ar, const unsigned int archive_version)

namespace proto {
namespace v1_0 {
static const version_type protocol_version = PROTOCOL_MAKE_VERSION(1, 0);

namespace ids {

namespace to_srv {

	enum Ids {
		Handshake_P3_accessrequest	= 0x10,

		Request_StdTileName			= 0x30,

		Control_KeyEvent			= 0x50
	};

}

namespace to_client {

	enum Ids {
		Handshake_P1_ProtocolVersion	= 0x70,
		Handshake_P2_ServerInformation	= 0x71,
		Handshake_P4_AcceptesGranted	= 0x72,

		LayerControl_AttachLayer		= 0x90,
		LayerControl_SendFrame			= 0x91,
		LayerConrtol_RemoveLayer		= 0x92,
		LayerControl_SendFullLayer		= 0x93,
		LayerControl_SendLayerUpdate	= 0x94,

		Answer_StdTileNameRequest			= 0xA0
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

PROTOCOL_MESSAGE(Request_StdTileName, to_srv)
{
	TNID tileId;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & tileId;
	}
};



PROTOCOL_MESSAGE(Control_KeyEvent, to_srv)
{
	enum EvtType
	{
		Evt_KeyDown,
		Evt_KeyUp
	};

	EvtType type;
	TNEVENT::keyevent_type data;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & type & data;
	}
};


PROTOCOL_MESSAGE(Answer_StdTileNameRequest, to_client)
{
	TNID tileId;
	string tileName;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & tileId & tileName;
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
	TNID commitNr;
	float xratio, yratio;
	uint16  width, height;
	std::vector<net::PTile> layerContent;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & layerId & commitNr & xratio & yratio & width & height & layerContent;
	}
};

PROTOCOL_MESSAGE(LayerControl_SendLayerUpdate, to_client)
{
	typedef std::pair<uint32, net::PTile> update_tile;

	TNID layerId;
	TNID commitNr;
	std::vector<update_tile> layerContent;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & layerId & commitNr & layerContent;
	}
};

PROTOCOL_MESSAGE(LayerControl_SendFrame, to_client)
{
	TNID layerId;
	TNID commitNr;
	bool is_delta;
	std::vector<TNID> sublayers_in_zorder;
	std::vector<std::pair<TNID, proto::v1_0::PView>> update_views;

	PROTOCOL_SERIALIZER(ar)
	{
		ar & layerId & commitNr & is_delta & sublayers_in_zorder & update_views;
	}
};

}

#undef PROTOCOL_THIS_VERSION
#undef PROTOCOL_MESSAGE
#undef PROTOCOL_SERIALIZER

#endif
