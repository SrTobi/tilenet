#pragma once
#ifndef _FS_PACKAGE_SOURCE_HPP
#define _FS_PACKAGE_SOURCE_HPP

#include "settings.hpp"
#include "virtual_package_source.hpp"


namespace vps {



class FsPackageSource
	: public VirtualPackageSource
{
public:
	FsPackageSource(const fs::path& path);
	FsPackageSource(const fs::path& path, const shared_ptr<FsPackageSource>& root);

	virtual OVERRIDE shared_ptr<VirtualPackageSource> root();

	virtual OVERRIDE shared_ptr<VirtualPackageSource> dir(const string& path);
	virtual OVERRIDE shared_ptr<VirtualPackageSource> loadVPS(const string& path);
	virtual OVERRIDE std::vector<wchar_t> loadText(const string& path);
	virtual OVERRIDE std::vector<byte> loadRaw(const string& path);

	virtual OVERRIDE bool isRoot();
	virtual OVERRIDE bool isValidPath(const string& path);

private:
	fs::path buildPath(const string& path);

	shared_ptr<FsPackageSource> mRoot;
	fs::path mPath;
};



}






#endif
