#include "includes.hpp"

#include "tile_mapper_v1_0.hpp"
#include "network/protocol.hpp"
#include "network/message.hpp"

#include "client/package/package.hpp"

namespace client {
namespace com {
namespace v1_0 {



TileMapper::TileMapper( const shared_ptr<net::ConnectionPort>& port )
	: mPort(port)
{
	tnAssert(port);
}

TileMapper::~TileMapper()
{
}

shared_ptr<StdTile> TileMapper::getStdTile(TNID tile_id ) const
{
	shared_ptr<StdTile> result;

	auto tile_it = mIdToStdTileMapping.find(tile_id);
	if(tile_it == mIdToStdTileMapping.end())
	{
		// Check if we can find a name
		auto name_it = mIdToNameMapping.find(tile_id);
		if(name_it == mIdToNameMapping.end())
		{
			// We haven't even a name
			// Ask the server for a name to resolve the id
			proto::v1_0::to_srv::Request_StdTileName req;
			req.tileId = tile_id;
			mPort->send(net::make_message(req));
		}else{
			// If a package is loaded, we can ask the package
			if(mPackage)
			{
				result = mPackage->getStdTileByName(name_it->second);
			}
		}
	}
	
	return result;
}


void TileMapper::identifyStdTile(const string& tile_name, TNID tile_id )
{
	mIdToNameMapping[tile_id] = tile_name;
}

void TileMapper::setPackage( const shared_ptr<Package>& p )
{
	mPackage = p;
}





}}}
