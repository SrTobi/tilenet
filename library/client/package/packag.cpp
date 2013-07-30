#include "includes.hpp"
#include "package.hpp"

#include "std_tile_pool.hpp"
#include "components/std_tile.hpp"

namespace client {



PackageInfo::PackageInfo(const string& name,
						 const string& version,
						 const string& pack_version,
						 const string& author,
						 const fs::path& path,
						 const std::vector<string>& interfaces )
	: mName(name)
	, mVersion(version)
	, mPackageVersion(pack_version)
	, mAuthor(author)
	, mPath(path)
	, mInterfaces(interfaces.begin(), interfaces.end())
{
}

const string& PackageInfo::name() const
{
	return mName;
}

const string& PackageInfo::version() const
{
	return mVersion;
}

const string& PackageInfo::packageVersion() const
{
	return mPackageVersion;
}

const string& PackageInfo::author() const
{
	return mAuthor;
}

const fs::path& PackageInfo::path() const
{
	return mPath;
}

bool PackageInfo::hasInterface( const string& interf) const
{
	return mInterfaces.find(interf) != mInterfaces.end();
}







Package::Package(const PackageInfo& pi, std::unordered_map<string, shared_ptr<StdTile>>&& tiles, std::unordered_map<string, Rect>&& tilesizes)
	: mInfo(pi)
	, mTileSizes(std::move(tilesizes))
	, mNameToStdTileMapping(std::move(tiles))
{
	auto it = mTileSizes.find(L"std");
	mStdTileSize = (it == mTileSizes.end())? StdTilePool::GetTileSize() : it->second;
}

Package::~Package()
{
}

shared_ptr<StdTile> Package::getStdTileByName( const string& name )
{
	auto it = mNameToStdTileMapping.find(name);

	if(it == mNameToStdTileMapping.end())
		return nullptr;

	return it->second;
}


const Rect& Package::getTileSize() const
{
	return mStdTileSize;
}


const Rect& Package::getTileSize(const string& name) const
{
	auto it = mTileSizes.find(name);

	return (it == mTileSizes.end())? getTileSize() : it->second;
}

const PackageInfo& Package::info()
{
	return mInfo;
}



}