#pragma once
#ifndef _ATTACH_LAYER_JOB_HPP
#define _ATTACH_LAYER_JOB_HPP


#include "settings.hpp"
#include "service_job.hpp"



namespace srv {

class Participant;
class Layer;

namespace job {

class AttachLayerJob
	: public ServiceJob
{
public:
	AttachLayerJob(const shared_ptr<Participant>& participant, const shared_ptr<Layer>& layer);
	~AttachLayerJob();

	virtual OVERRIDE void process();

private:
	const shared_ptr<Participant> mParticipant;
	const shared_ptr<Layer> mLayer;
};



}}









#endif