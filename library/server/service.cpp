#include "includes.hpp"
#include "server/service.hpp"
#include "utils/thread_pool.hpp"




namespace srv {

std::unique_ptr<Service> Service::Singleton;

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


bool Service::IsInstanced()
{
	return Singleton.get()? true : false;
}

Service& Service::Inst()
{
	if(!IsInstanced())
		Singleton.reset(new Service());

	return *Singleton;
}

void Service::Shutdown()
{
	Singleton.reset();
}


}
