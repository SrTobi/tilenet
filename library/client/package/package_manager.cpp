#include "includes.hpp"
#include "package_manager.hpp"

#include "rapidxml.hpp"
#include <boost/filesystem/operations.hpp>


namespace client {


class PackageManager::PackageFinder
{
public:
	template<typename Iter>
	PackageFinder(Iter begin, Iter end)
	{
		std::copy(begin, end, std::back_inserter(mPaths));
	}


	void extract_package_info_from_debug_file_format(const fs::path& path)
	{
		std::vector<wchar_t> buffer;
		{
			std::wifstream file(path.string());

			if(!file.is_open())
				return;
			file >> std::noskipws;
			std::copy(std::istream_iterator<wchar_t, wchar_t>(file), std::istream_iterator<wchar_t, wchar_t>(), std::back_inserter(buffer));
		}

		rapidxml::xml_document<wchar_t> doc;
		doc.parse<	rapidxml::parse_trim_whitespace |
					rapidxml::parse_comment_nodes |
					rapidxml::parse_validate_closing_tags |
					rapidxml::parse_normalize_whitespace |
					rapidxml::parse_no_data_nodes>(buffer.data());

		auto* info_node = doc.first_node(L"info");

		if(!info_node)
			return;

		
	}


	void checkFile(const fs::path& path)
	{
		extract_package_info_from_debug_file_format(path);
	}


	void iterateDirectory(const fs::path& path)
	{
		for(auto& p : path)
		{
			if(fs::is_directory(p))
			{
				iterateDirectory(p);
			}else if(fs::is_regular_file(p) && p.extension() == ".tnpack")
			{
				checkFile(p);
			}
		}
	}


	std::vector<PackageInfo> search()
	{
		for(auto& path : mPaths)
		{
			iterateDirectory(path);
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

std::future<shared_ptr<Package>> PackageManager::loadPackageByName( const string& name )
{
	NOT_IMPLEMENTED();
}

std::future<shared_ptr<Package>> PackageManager::loadPackageByInterface( const string& interface_name )
{
	NOT_IMPLEMENTED();
}

std::future<shared_ptr<Package>> PackageManager::loadPackageByPath( const fs::path& path )
{
	NOT_IMPLEMENTED();
}

std::future<shared_ptr<Package>> PackageManager::loadPackageByPackageInfo( const PackageInfo& info )
{
	NOT_IMPLEMENTED();
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

	auto func = std::bind(&PackageFinder::search, pf);
	std::packaged_task<std::vector<PackageInfo>()> task(func);
	mPackageInfosWaiter = task.get_future();

	std::thread(std::move(task)).detach();
}




}