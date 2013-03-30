#include "includes.hpp"
#include "server/service.hpp"
#include "utils/thread_pool.hpp"




namespace srv {

Service::Service()
{
	mThreadPool.reset(new ThreadPool(*this));
	mThreadPool->setThreadCount(1);
}

Service::~Service()
{

}

void Service::setThreadCount(size_t count)
{
	mThreadPool->setThreadCount(count);
}


void Service::shutdown()
{
}


Service& Service::Inst()
{
	static Service serviceSingleton;
	return serviceSingleton;
}

}