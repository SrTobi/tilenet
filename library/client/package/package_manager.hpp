#pragma once
#ifndef _PACKAGE_MANAGER_HPP
#define _PACKAGE_MANAGER_HPP

#include <future>
#include "settings.hpp"
#include "package.hpp"

namespace client {


class PackageManager
{
private:
	class PackageFinder;

public:
	PackageManager();
	~PackageManager();

	shared_ptr<Package> loadPackageByName(const string& name);
	shared_ptr<Package> loadPackageByInterface(const string& interface_name);
	shared_ptr<Package> loadPackageByPath(const fs::path& path);
	shared_ptr<Package> loadPackageByPackageInfo(const PackageInfo& info);

	const std::vector<PackageInfo>& getPackageInfos();
	void addPackagePath(const fs::path& path);
	void serachPackages();
	bool isSearching();

private:
	std::unordered_set<fs::path> mPackagePaths;
	std::vector<PackageInfo> mPackageInfos;
	std::future<std::vector<PackageInfo>> mPackageInfosWaiter;
};



}


#endif
