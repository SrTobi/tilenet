#include "includes.hpp"

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <thread>

#include "package_manager.hpp"
#include "loader/package_loader.hpp"

namespace client {


class PackageManager::PackageFinder
{
public:
	Log log;

	template<typename Iter>
	PackageFinder(Iter begin, Iter end)
		: log(L"pack-search")
	{
		std::copy(begin, end, std::back_inserter(mPaths));
	}


	void checkPackageDotXml(const fs::path& path)
	{
		log.info() << L"Found package " << path.wstring();

		try {
			mResult.push_back(pload::loadPackageInfo(path));
		}catch(...)
		{
		}
	}


	void iterateDirectory(const fs::path& path, unsigned int deep)
	{
		if(deep <= 0)
			return;

		for(fs::directory_iterator it(path); it != fs::directory_iterator(); ++it)
		{
			const fs::path& p = it->path();

			if(fs::is_directory(p))
			{
				iterateDirectory(p, deep - 1);
			}else if(fs::is_regular_file(p) && p.filename() == "package.xml")
			{
				checkPackageDotXml(p.parent_path());
			}
		}
	}


	std::vector<PackageInfo> search(unsigned int deep)
	{
		log.info() << L"Start searching for packages...";
		for(auto& path : mPaths)
		{
			iterateDirectory(path, deep);
		}

		if(mResult.empty())
			log.error() << "No packages found!";

		return std::move(mResult);
	}

private:
	std::vector<PackageInfo> mResult;
	std::vector<fs::path> mPaths;
};





PackageManager::PackageManager()
{
}

PackageManager::~PackageManager()
{

}

shared_ptr<Package> PackageManager::loadPackageByName( const string& name )
{
	auto& packageInfos = getPackageInfos();

	for(auto& pi : packageInfos)
	{
		if(pi.name() == name)
		{
			return loadPackageByPackageInfo(pi);
		}
	}

	return nullptr;
}

shared_ptr<Package> PackageManager::loadPackageByInterface( const string& interface_name )
{
	auto& packageInfos = getPackageInfos();

	for(auto& pi : packageInfos)
	{
		if(pi.hasInterface(interface_name))
		{
			return loadPackageByPackageInfo(pi);
		}
	}

	return nullptr;
}

shared_ptr<Package> PackageManager::loadPackageByPath( const fs::path& path )
{
	try {
		return pload::loadPackage(path);

	}catch(excp::ExceptionBase& e)
	{
		Log().error() << "Failed to load package[" << path.wstring() << "]: " << e.why();
	}catch(...)
	{
		Log().error() << "Failed to load package[" << path.wstring() << "]";
	}

	return nullptr;
}

shared_ptr<Package> PackageManager::loadPackageByPackageInfo( const PackageInfo& info )
{
	return loadPackageByPath(info.path());
}

const std::vector<PackageInfo>& PackageManager::getPackageInfos()
{
	if(mPackageInfos.empty() && mPackageInfosWaiter.valid())
	{
		mPackageInfos = mPackageInfosWaiter.get();
	}
	return mPackageInfos;
}

void PackageManager::addPackagePath( const fs::path& path )
{
	mPackagePaths.insert(path);
}

void PackageManager::serachPackages()
{
	mPackageInfos.clear();
	if(mPackagePaths.empty())
		return;

	std::shared_ptr<PackageFinder> pf = std::make_shared<PackageFinder>(mPackagePaths.begin(), mPackagePaths.end());

	auto func = std::bind(&PackageFinder::search, pf, 3);
	boost::packaged_task<std::vector<PackageInfo>> task(std::move(func));
	mPackageInfosWaiter = task.get_future();

	std::thread(std::move(task)).detach();
}

bool PackageManager::isSearching()
{
	return mPackageInfosWaiter.valid() && mPackageInfosWaiter.get_state() != boost::future_state::ready;
}




}
