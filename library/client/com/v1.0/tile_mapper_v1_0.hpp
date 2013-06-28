#pragma once
#ifndef _TILE_MANAGER_HPP
#define _TILE_MANAGER_HPP


#include <unordered_map>

#include "settings.hpp"
#include "network/connection_port.hpp"


namespace client {

class StdTile;
class Package;

namespace com {
namespace v1_0 {

class TileMapper
{
public:
	class StdIdTileset;
public:
	TileMapper(const shared_ptr<net::ConnectionPort>& port);
	~TileMapper();

	shared_ptr<StdTile> getStdTile(TNID tile_id) const;
	
	void identifyStdTile(const string& tile_name, TNID tile_id);
	void setPackage(const shared_ptr<Package>& p);
private:
	mutable std::unordered_set<TNID>				mAlreadyAskedIds;
	std::unordered_map<TNID, string>				mIdToNameMapping;
	std::unordered_map<TNID, shared_ptr<StdTile>>	mIdToStdTileMapping;
	shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<Package> mPackage;
};



}}}






#endif