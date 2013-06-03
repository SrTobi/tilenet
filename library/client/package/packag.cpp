#include "includes.hpp"
#include "package.hpp"

namespace client {



PackageInfo::PackageInfo(const string& name,
						 const string& version,
						 const string& pack_version,
						 const string& author,
						 const string& path,
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







Package::Package()
{
	NOT_IMPLEMENTED()
}

Package::~Package()
{
	NOT_IMPLEMENTED()
}

shared_ptr<StdTile> Package::getStdTileByName( const string& name )
{
	NOT_IMPLEMENTED()
}

const PackageInfo& Package::info()
{
	NOT_IMPLEMENTED()
}



}