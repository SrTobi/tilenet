#pragma once
#ifndef _PACKAGE_HPP
#define _PACKAGE_HPP


#include <hash_set>
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
				const string& path,
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
	std::hash_set<string> mInterfaces;
};






class Package
{
public:
	Package();
	~Package();

	shared_ptr<StdTile> getStdTileByName(const string& name);


	const PackageInfo& info();
};


}



#endif