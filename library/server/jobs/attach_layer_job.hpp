#pragma once
#ifndef _ATTACH_LAYER_JOB_HPP
#define _ATTACH_LAYER_JOB_HPP


#include "settings.hpp"
#include "service_job.hpp"



namespace srv {


class AttachLayerJob
	: public ServiceJob
{
public:
	AttachLayerJob();
	~AttachLayerJob();

	virtual OVERRIDE void process();

private:
};



}









#endif