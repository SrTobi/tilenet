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







Package::Package(const PackageInfo& pi)
	: mInfo(pi)
	, mPackageTileSize(8, 12)
{
	auto filename = pi.path() / "debug-tileset.txt";

	Log loadLog(L"tile-load");

	loadLog.debug() << L"Load debug tileset from \"" << filename << "\"";

	std::wifstream file(filename.string());

	if(!file.is_open())
	{
		loadLog.error() << "Failed to load tile source!";
		return;
	}

	const auto& pool = StdTilePool::Inst();


	string line;
	int lineNum = 1;

	while(std::getline(file, line))
	{
		std::wistringstream iss(line);

		// skip comments
		if(line.size() && line.find(L"//") == string::npos)
		{
			string name;
			string std_tile_name;
			iss >> name >> std_tile_name;

			if(iss.fail() || !iss.eof())
			{
				loadLog.error() << L"Failed to parse line " << lineNum;
				return;
			}

			auto sprite = pool.getStdTile(std_tile_name);

			mNameToStdTileMapping[name] = sprite;
		}

		++lineNum;
	}

	loadLog.debug() << mNameToStdTileMapping.size() << " tiles loaded";
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
	return mPackageTileSize;
}

const PackageInfo& Package::info()
{
	return mInfo;
}



}