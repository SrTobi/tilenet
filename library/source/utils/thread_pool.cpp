#include "includes.hpp"
#include "utils/thread_pool.hpp"



using boost::asio::io_service;

ThreadPool::ThreadPool(io_service& ioService)
	: mTargetThreadCount(0)
	, mDestroy(false)
	, mService(ioService)
	, log(L"ThreadPool")
{
}

ThreadPool::~ThreadPool()
{
	{
		// Hmmm, a lock will cause an exception on exit cleanup
		// std::lock_guard<std::mutex> lock(mMutex);
		mDestroy = true;
		mWork.reset();
	}

	for(auto& p : mThreads)
	{
		p.second.join();
	}
}

void ThreadPool::setThreadCount(size_t count)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mTargetThreadCount == count)
		return;

	mTargetThreadCount = count;

	if(mTargetThreadCount < count)
	{
		while(mThreads.size() < count)
		{
			std::thread thread(std::bind(&ThreadPool::runThread, this));
			mThreads.insert(std::make_pair(thread.get_id(), std::move(thread)));
		}
	}else{
		mWork.reset();
	}
}


void ThreadPool::runThread()
{
	bool need = true;
	while(need)
	{
		mService.run();

		std::lock_guard<std::mutex> lock(mMutex);

		if(mDestroy)
			return;

		if(mThreads.size() > mTargetThreadCount)
		{
			need = false;
			auto it = mThreads.find(std::this_thread::get_id());
			tnAssert(it != mThreads.end());
			it->second.detach();
			mThreads.erase(it);
		}else{
			if(!mWork)
			{
				mWork.reset(new io_service::work(mService));
			}
		}
		log.info() << L"thread count is now " << mThreads.size();
	}
}