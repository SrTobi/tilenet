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
	, mNameToStdTileMapping(std::move(tiles))
{
	auto it = tilesizes.find(L"std");
	mStdAspect = (it == tilesizes.end())? StdTilePool::GetTileSize() : it->second;

	// calculate aspects
	std::copy(tilesizes.begin(), tilesizes.end(), std::inserter(mAspects, mAspects.begin()));
	mAspects[L"std"] = mStdAspect;
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



const Ratio& Package::getAspect(const string& name) const
{
	auto it = mAspects.find(name);

	return (it == mAspects.end())? mStdAspect : it->second;
}

const PackageInfo& Package::info()
{
	return mInfo;
}



}