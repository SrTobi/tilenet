#pragma once
#ifndef _SERVICE_HPP
#define _SERVICE_HPP

#include <map>
#include <boost/asio/io_service.hpp>
#include "settings.hpp"


namespace srv {

class Service
	: public boost::asio::io_service
{
public:
	~Service();

	void setThreadCount(size_t count);
	void shutdown();

	static Service& Inst();
private:
	Service();
	void runThread();

private:
	std::mutex mMutex;
	size_t mTargetThreadCount;
	std::map<std::thread::id, std::thread> mThreads;
	std::unique_ptr<boost::asio::io_service::work> mWork;
	bool mDestroy;
};


}



#endif