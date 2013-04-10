#pragma once
#ifndef _THREAD_POOL_HPP
#define _THREAD_POOL_HPP


#include <boost/asio/io_service.hpp>

#include "settings.hpp"

class ThreadPool
{
public:
	ThreadPool(boost::asio::io_service& ioService);
	~ThreadPool();

	void setThreadCount(size_t count);
private:
	void runThread();

private:
	std::mutex mMutex;
	size_t mTargetThreadCount;
	std::map<std::thread::id, std::thread> mThreads;
	std::unique_ptr<boost::asio::io_service::work> mWork;
	bool mDestroy;
	boost::asio::io_service& mService;
	Log log;
};




#endif