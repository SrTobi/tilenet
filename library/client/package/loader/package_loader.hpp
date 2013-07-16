#pragma once
#ifndef _PACKAGE_LOADER_HPP
#define _PACKAGE_LOADER_HPP


#include "settings.hpp"
#include "client/package/package.hpp"


namespace client {
namespace pload {




shared_ptr<Package> loadPackage(const fs::path& ppath);


}}




#endif