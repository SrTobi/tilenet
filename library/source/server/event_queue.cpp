#include "includes.hpp"
#include "server/event_queue.hpp"




namespace srv {

EventQueue::EventQueue()
{
}

EventQueue::~EventQueue()
{
}

void EventQueue::push(const TNEVENT& event)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mQueue.push(event);
	mSignal.notify_one();
}

void EventQueue::pop(TNEVENT* event)
{
	std::unique_lock<std::mutex> lock(mMutex);

	while(mQueue.empty())
	{
		mSignal.wait(lock);
	}
	*event = mQueue.front();
	mQueue.pop();
}

bool EventQueue::pop(TNEVENT* event, std::chrono::milliseconds timeout)
{
	std::unique_lock<std::mutex> lock(mMutex);

	auto start_time = std::chrono::system_clock::now();
	while(mQueue.empty())
	{
		 if(mSignal.wait_for(lock, timeout) == std::cv_status::timeout)
		 {
			 return false;
		 }

		 timeout -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time); 
	}


	*event = mQueue.front();
	mQueue.pop();

	return true;
}







}