#include "includes.h"
#include "client/loader/packageManager.h"




namespace client {


PackageManager::PackageManager()
{

}

PackageManager::~PackageManager()
{

}



PackageManager& PackageManager::Inst()
{
	static PackageManager inst;
	return inst;
}



}