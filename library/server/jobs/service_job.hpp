#pragma once
#ifndef _SERVICE_JOB_HPP
#define _SERVICE_JOB_HPP


#include "settings.hpp"



namespace srv {

class ServiceJob
{
public:
	ServiceJob();
	~ServiceJob();

	virtual void process() = 0;
};


}


#endif