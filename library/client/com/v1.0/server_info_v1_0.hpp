#pragma once
#ifndef _SERVER_INFO_V1_0_HPP
#define _SERVER_INFO_V1_0_HPP

#include "settings.hpp"

namespace client {
namespace com {
namespace v1_0 {


class ServerInfo
{
public:
	ServerInfo(const string& name, const string& info, const string& pack_name, const string& pack_interface);

	const string& name() const;
	const string& info() const;
	const string& packageName() const;
	const string& packageInterface() const;

private:
	string mServerName;
	string mServerInfo;
	string mPackageName;
	string mPackageInterface;
};





}}}


#endif