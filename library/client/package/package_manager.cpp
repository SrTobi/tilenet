#include "includes.hpp"

#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <thread>

#include "package_manager.hpp"

#include "rapidxml.hpp"
#include "utils/xml_helpers.hpp"

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


	void extract_package_info_from_debug_file_format(const fs::path& path)
	{
		log.info() << L"Found package " << path.wstring();

		std::vector<wchar_t> buffer;
		{
			std::wifstream file(path.string());

			if(!file.is_open())
				return;
			file >> std::noskipws;
			std::copy(std::istream_iterator<wchar_t, wchar_t>(file), std::istream_iterator<wchar_t, wchar_t>(), std::back_inserter(buffer));
			buffer.push_back(0);
		}

		rapidxml::xml_document<wchar_t> doc;
		try {
			doc.parse<	rapidxml::parse_trim_whitespace |
						rapidxml::parse_comment_nodes |
						rapidxml::parse_validate_closing_tags |
						rapidxml::parse_normalize_whitespace>(buffer.data());
		} catch(rapidxml::parse_error& e)
		{
			log.error() << L"Failed to load package [" << path << L"]:" << e.what();
			return;
		}

		auto* info_node = xml::get_node<wchar_t>(&doc, L"package/information");

		tnAssert(info_node);

		mResult.push_back(	PackageInfo(xml::get_attr<string, wchar_t>(info_node, L"", L"name"),
										L"1.0.0.0",
										L"1.0.0.0",
										xml::get_attr<string, wchar_t>(info_node, L"author", L"name", L"unknown"),
										fs::canonical(path).parent_path(),
										std::vector<string>()));
	}


	void checkFile(const fs::path& path)
	{
		extract_package_info_from_debug_file_format(path);
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
				checkFile(p);
			}
		}
	}


	std::vector<PackageInfo> search(unsigned int deep)
	{
		for(auto& path : mPaths)
		{
			iterateDirectory(path, deep);
		}


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
	NOT_IMPLEMENTED();
}

shared_ptr<Package> PackageManager::loadPackageByPackageInfo( const PackageInfo& info )
{
	return std::make_shared<Package>(info);
}

const std::vector<PackageInfo>& PackageManager::getPackageInfos()
{
	if(mPackageInfosWaiter.valid())
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

	auto func = std::bind(&PackageFinder::search, pf, 2);
	boost::packaged_task<std::vector<PackageInfo>> task(func);
	mPackageInfosWaiter = task.get_future();

	std::thread(std::move(task)).detach();
}

bool PackageManager::isSearching()
{
	return mPackageInfosWaiter.valid() && mPackageInfosWaiter.get_state() != boost::future_state::ready;
}




}
