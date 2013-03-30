#pragma once
#ifndef _SERVICE_HPP
#define _SERVICE_HPP

#include <map>
#include <boost/asio/io_service.hpp>
#include "settings.hpp"


class ThreadPool;

namespace srv {

class Service
	: public boost::asio::io_service
{
public:
	~Service();

	void setThreadCount(size_t count);

	static bool IsInstanced();
	static Service& Inst();
	static void Shutdown();
private:
	Service();

private:
	std::unique_ptr<ThreadPool> mThreadPool;

	static std::unique_ptr<Service> Singleton;
};


}



#endif