#include "includes.hpp"
#include "server/service.hpp"

namespace srv {

srv::Service::Service()
	: mTargetThreadCount(0)
	, mDestroy(false)
{
}

srv::Service::~Service()
{

}

void srv::Service::setThreadCount(size_t count)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mTargetThreadCount == count)
		return;

	mTargetThreadCount = count;

	if(mTargetThreadCount < count)
	{
		while(mThreads.size() < count)
		{
			std::thread thread(std::bind(&Service::runThread, self<Service>()));
			mThreads.insert(std::make_pair(thread.get_id(), std::move(thread)));
		}
	}else{
		mWork.reset();
	}
}


void srv::Service::destroy()
{
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mDestroy = true;
		mWork.reset();
	}

	for(auto& p : mThreads)
	{
		p.second.join();
	}
}

shared_ptr<TilenetObject> srv::Service::clone()
{
	shared_ptr<Service> newService(new Service());

	newService->setThreadCount(mTargetThreadCount);

	return newService;
}



void Service::runThread()
{
	bool need = true;
	while(need)
	{
		io_service::run();

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
				mWork.reset(new work(*this));
			}
		}
	}
}

}