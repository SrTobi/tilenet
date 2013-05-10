#pragma once
#ifndef _SEND_FULL_LAYER_JOB_HPP
#define _SEND_FULL_LAYER_JOB_HPP


#include "settings.hpp"

#include "service_job.hpp"


namespace srv {

class Layer;

namespace job {


class SendFullLayerJob
	: public ServiceJob
{
public:
	SendFullLayerJob(const shared_ptr<Layer>& layer);
	~SendFullLayerJob();


	virtual OVERRIDE void process();

private:
	shared_ptr<Layer> mLayer;
};






}}




#endif