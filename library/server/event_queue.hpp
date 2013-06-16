#pragma once
#ifndef _EVENT_QUEUE_HPP
#define _EVENT_QUEUE_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "settings.hpp"



namespace srv {
	
	
	
class EventQueue
{
public:
	EventQueue();
	~EventQueue();

	void push(const TNEVENT& event);
	void pop(TNEVENT* event);
	bool pop(TNEVENT* event, std::chrono::milliseconds& timeout);


private:
	std::queue<TNEVENT>		mQueue;
	std::mutex				mMutex;
	std::condition_variable mSignal;
};


}


#endif