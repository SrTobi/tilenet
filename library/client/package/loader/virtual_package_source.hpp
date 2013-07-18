#pragma once
#ifndef _VIRTUAL_PACKAGE_SOURCE_HPP
#define _VIRTUAL_PACKAGE_SOURCE_HPP

#include <vector>
#include "settings.hpp"


namespace vps {



class VirtualPackageSource
	: public std::enable_shared_from_this<VirtualPackageSource>
{
public:
	virtual shared_ptr<VirtualPackageSource> root() = 0;

	virtual shared_ptr<VirtualPackageSource> dir(const string& path) = 0;
	virtual shared_ptr<VirtualPackageSource> loadVPS(const string& path) = 0;
	virtual std::vector<wchar_t> loadText(const string& path) = 0;
	virtual std::vector<byte> loadRaw(const string& path) = 0;

	virtual bool isRoot() = 0;
	virtual bool isValidPath(const string& path) = 0;
	virtual string srcname() = 0;
};



shared_ptr<VirtualPackageSource> LoadVPSFromSF(const fs::path& path);






}






#endif
