#pragma once
#ifndef _NETWORK_H
#define _NETWORK_H


#include "settings.h"

#include "obuffer.h"
#include "ibuffer.h"

namespace net {
	
	
bool send(const socket_ptr& socket, const OBuffer::Map& map);
bool send(const socket_ptr& socket, const OBuffer& buf);
bool send(asio::tcp::socket& socket, const OBuffer::Map& map);
bool send(asio::tcp::socket& socket, const OBuffer& buf);



/*
 *
 *	
 *
 *
 *
 *
 *
 *
 *
 *
 */
namespace msg {

static const sf::Uint8 Error		= 0;
static const sf::Uint8 Collection	= 1;


namespace ToClient {
/*
 *	Teilt mit, dass ein Layer erzeugt oder geaendert werden muss.
 *	Format:
 *		uint	- id des layers
 *		int*int	- Position des Layers
 *		uint
 *		int*int - Groesse des Layers
 *		color	- Hintergrundfarbe des Layers
 *		uint	- id des layers, welcher darueber liegt oder 0, wenn darueber kein layer ist
 *
 */
static const sf::Uint8 SendLayerProperties = 2;

/*
 *	Sendet den Inhalt eines layers, der geupdated wurde.
 *	Format:
 *		uint	- id des Layers
 *		uint	- anzahl der geupdatetn tiles
 *		{
 *			uint	- relative Position zum letzten tile
 *			uint	- tileid
 *			uint	- farbe
 *		}
 *
 */

static const sf::Uint8 SendUpdatedLayerContent = 3;


/*
 *	Sendet den gesamten Inhalt eines layers.
 *	Format:
 *		uint	- id des Layers
 *		uint	- anzahl der tiles
 *		{
 *			uint	- tileid
 *			uint	- farbe
 *		}
 *
 */

static const sf::Uint8 SendFullLayerContent = 4;

/*
 *	Sendet die Daten, die ein Tile definieren
 *	Format:
 *		uint	- id des tiles
 *		nstring	- name des tiles
 */
static const sf::Uint8 SendTileDefinition = 5;
}

namespace ToServer {

/*
 *	Fragt nach einer genauen Definition eines tiles
 *
 *	uint	- id des tiles das hinterfragt werden soll
 */
static const sf::Uint8 RequestTileDef = 30;

/*
 *	Sendet ein Tastenevent
 *	Format:
 *		byte			- Optionen [charakter oder buchstabe, system-taste, alt-taste, shift-taste, steuer-taste]
 *		wchar(32Bit)	- keycode/unicode-code
 */
static const sf::Uint8 SendKeyEvent = 31;
}

}



template<sf::Uint8 PID>
class ProtocolBase
{
public:
	static const sf::Uint8 pid = PID;
public:
	ProtocolBase(const IBuffer& buf)
	{
		if(buf.id() != PID)
			STHROW(L"Buffer has not the right id!");
	}

};


template<sf::Uint8 PID>
class Protocol
{  };

template<>
class Protocol<msg::Collection>
	: public ProtocolBase<msg::Collection>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf >> collection;
	}
	


	OBuffer::Map	collection;

	static void Inject(OBuffer& buffer, const OBuffer::Map& map)
	{
		buffer.id(pid);
		buffer << map;
	}
};

template<>
class Protocol<net::msg::ToClient::SendLayerProperties>
	: public ProtocolBase<net::msg::ToClient::SendLayerProperties>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> layerId
			>> position >> posRatio
			>> size >> sizeRatio
			>> bgcolor
			>> overlyingLayer;
	}

	nuint		layerId;
	tn::Point	position;
	tn::Ratio	posRatio;
	tn::Rect	size;
	tn::Rect	sizeRatio;
	nuint		bgcolor;
	nuint		overlyingLayer;


	static void Inject(OBuffer& buffer, nuint layerId, Point pos, Ratio posRatio,
						Rect size, Ratio sizeRatio, nuint bgcolor, nuint overlyingLayer)
	{
		buffer	<< layerId
				<< pos << posRatio
				<< size << sizeRatio
				<< bgcolor
				<< overlyingLayer;
	}
};







template<>
class Protocol<net::msg::ToClient::SendUpdatedLayerContent>
	: public ProtocolBase<net::msg::ToClient::SendUpdatedLayerContent>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> layerId
			>> count;
	}

	nuint layerId;
	nuint count;


	static void Inject(OBuffer& buffer, nuint layerId, nuint count)
	{
		buffer	<< layerId
				<< count;
	}
};


template<>
class Protocol<net::msg::ToClient::SendFullLayerContent>
	: public ProtocolBase<net::msg::ToClient::SendFullLayerContent>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> layerId
			>> count;
	}

	nuint layerId;
	nuint count;


	static void Inject(OBuffer& buffer, nuint layerId, nuint count)
	{
		buffer	<< layerId
				<< count;
	}
};

	
template<>
class Protocol<net::msg::ToClient::SendTileDefinition>
	: public ProtocolBase<net::msg::ToClient::SendTileDefinition>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> tileId >> tileName;
	}

	nuint	tileId;
	nstring	tileName;

	static void Inject(OBuffer& buffer, nuint tileId, const nstring& tileName)
	{
		buffer	<< tileId << tileName;
	}
};


template<>
class Protocol<net::msg::ToServer::RequestTileDef>
	: public ProtocolBase<net::msg::ToServer::RequestTileDef>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> tileId;
	}

	nuint tileId;


	static void Inject(OBuffer& buffer, nuint tileId)
	{
		buffer	<< tileId;
	}
};


template<>
class Protocol<net::msg::ToServer::SendKeyEvent>
	: public ProtocolBase<net::msg::ToServer::SendKeyEvent>
{
public:
	Protocol(IBuffer& buf)
		: ProtocolBase(buf)
	{
		buf	>> option >> keycode;
	}

	byte	option;
	wchar	keycode;


	static void Inject(OBuffer& buffer, byte option, wchar keycode)
	{
		buffer	<< option << keycode;
	}
};

/*

class NetPack
{
public:
	virtual void extract(sf::Packet&);

	inline nuint id() const { return mNetId; }

	void checkPos(const Point& pos);
	void checkSize(const Rect& size);
	
	nuint mNetId;
};


template<nuint PackID>
class Pack
{int _Error[-1];};


static const nuint ErrorBit						= 0x80000000;


namespace pending {
static const nuint _Start						= 100;
namespace ToClient {
	static const nuint ServerIsFull				= 101;
	static const nuint PasswordIsWrong			= 102;
	static const nuint ServerGrantsPlayer		= 103;	// (Packagename:string)
	static const nuint ServerGrantsObserver		= 104;	// (Packagename:string)
}
namespace ToServer {
	static const nuint ClientWantsToBeObserver	= 121;	// Observerpassword:string
	static const nuint ClientWantsToBePlayer		= 122;	// playerpassword:string
}
static const nuint _End							= 150;
}

namespace graphic {
static const nuint _Start						= 200;
namespace ToClient {
	static const nuint CreateLayer				= 201;	// id:nuint, pos:Point, size:Rect, bgcolor:nuint (, above:nuint)
	static const nuint PutTile					= 202;	// layerId:nuint, pos:Point, tileId:nuint, tileColor:nuint
	static const nuint SendTileDef				= 203;	// id:nuint, name:string
}
namespace ToServer {
	static const nuint RequestTileDef			= 221; // id:nuint
}
static const nuint _End							= 250;
}



//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////     pending::ToClient        /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


template<>
class Pack<pending::ToClient::ServerIsFull>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToClient::ServerIsFull;
	}
};


template<>
class Pack<pending::ToClient::PasswordIsWrong>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToClient::PasswordIsWrong;
	}
};


template<>
class Pack<pending::ToClient::ServerGrantsPlayer>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToClient::ServerGrantsPlayer;
	}
};


template<>
class Pack<pending::ToClient::ServerGrantsObserver>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToClient::ServerGrantsObserver;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////     pending::ToServer        /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

template<>
class Pack<pending::ToServer::ClientWantsToBeObserver>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToServer::ClientWantsToBeObserver;
	}
	
	nstring password;

protected:	
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> password;
	}
};


template<>
class Pack<pending::ToServer::ClientWantsToBePlayer>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack)
	{
		pack << pending::ToServer::ClientWantsToBePlayer;
	}
	
	nstring password;

protected:	
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> password;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    graphic::ToServer         /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

template<>
class Pack<graphic::ToServer::RequestTileDef>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack, nuint tileId)
	{
		pack << graphic::ToServer::RequestTileDef << tileId;
	}

	// Die id des Tiles, welches nachgefragt wird
	nuint tileId;

	
protected:
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> tileId;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////    graphic::ToClient         /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


template<>
class Pack<graphic::ToClient::CreateLayer>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack, nuint layerId, Point pos,
								Rect size, nuint bgcolor, nuint overlyingLayer)
	{
		pack << graphic::ToClient::CreateLayer << layerId << pos << size
				<< bgcolor << overlyingLayer;
	}

	// Die id des Tiles, welches nachgefragt wird
	nuint	newLayerId;
	Point	position;
	Rect	size;
	nuint	bgcolor;
	nuint	overlyingLayer;
	
protected:
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> newLayerId
				>> position
				>> size
				>> bgcolor
				>> overlyingLayer;
		checkSize(size);
		checkPos(position);
	}
};


template<>
class Pack<graphic::ToClient::PutTile>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack, nuint layerId, Point pos,
								nuint tileId, nuint tileColor)
	{
		pack << graphic::ToClient::PutTile << layerId << pos
				<< tileId << tileColor;
	}

	// Die id des Tiles, welches nachgefragt wird
	nuint	targetLayerId;
	Point	position;
	nuint	tileId;
	nuint	tileColor;
	
protected:
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> targetLayerId
				>> position
				>> tileId
				>> tileColor;
		checkPos(position);
	}
};


template<>
class Pack<graphic::ToClient::SendTileDef>
	: public NetPack
{
public:
	inline static void Inject(sf::Packet& pack, nuint tileId, const nstring& tileName)
	{
		pack << graphic::ToClient::SendTileDef << tileId << tileName;
	}

	// Die id des Tiles, welches nachgefragt wird
	nuint	tileId;
	nstring	tileName;
	
protected:
	inline virtual void extract(sf::Packet& packet)
	{
		packet >> tileId >> tileName;
	}
};
*/







}


#endif
