#include "includes.hpp"
#include "server_info_v1_0.hpp"



namespace client {
namespace com {
namespace v1_0 {


ServerInfo::ServerInfo( const string& name, const string& info, const string& pack_name, const string& pack_interface )
	: mServerName(name)
	, mServerInfo(info)
	, mPackageName(pack_name)
	, mPackageInterface(pack_interface)
{
}

const string& ServerInfo::name() const
{
	return mServerName;
}

const string& ServerInfo::info() const
{
	return mServerInfo;
}

const string& ServerInfo::packageName() const
{
	return mPackageName;
}

const string& ServerInfo::packageInterface() const
{
	return mPackageInterface;
}



}}}