#pragma once
#ifndef _PACKAGE_HPP
#define _PACKAGE_HPP


#include <unordered_set>
#include <unordered_map>
#include "settings.hpp"


namespace client {

class StdTile;


class PackageInfo
{
public:
	PackageInfo(const string& name,
				const string& version,
				const string& pack_version,
				const string& author,
				const fs::path& path,
				const std::vector<string>& interfaces);

	const string& name() const;
	const string& version() const;
	const string& packageVersion() const;
	const string& author() const;
	const fs::path& path() const;

	bool hasInterface(const string& interf) const;

private:
	string mName;
	string mVersion;
	string mPackageVersion;
	string mAuthor;
	fs::path mPath;
	std::unordered_set<string> mInterfaces;
};






class Package
{
public:
	Package(const PackageInfo& pi);
	~Package();

	shared_ptr<StdTile> getStdTileByName(const string& name);


	const PackageInfo& info();

private:
	std::unordered_map<string, shared_ptr<StdTile>> mNameToStdTileMapping;
	PackageInfo mInfo;
};


}



#endif
