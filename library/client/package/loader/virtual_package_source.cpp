#include "includes.hpp"
#include "virtual_package_source.hpp"

#include <boost/filesystem.hpp>

#include "fs_package_source.hpp"

namespace vps {





shared_ptr<VirtualPackageSource> LoadVPSFromSF( const fs::path& path )
{
	fs::path cpath = fs::canonical(path);

	if(fs::is_directory(cpath))
	{
		return std::make_shared<FsPackageSource>(cpath);
	}


	BOOST_THROW_EXCEPTION(excp::VPSExpection() << excp::InfoWhat(L"Can not open virtual source[" + path.wstring() + L"]"));
}

}
