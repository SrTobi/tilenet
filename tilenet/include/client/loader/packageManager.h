#pragma once
#ifndef _PACKAGELOADER_H
#define _PACKAGELOADER_H




namespace client {

class PackageManager
{
public:
	PackageManager();
	~PackageManager();

	static PackageManager& Inst();
private:
};




}





#endif