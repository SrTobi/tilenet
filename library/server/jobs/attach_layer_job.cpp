#include "includes.hpp"
#include "attach_layer_job.hpp"



namespace srv {
namespace job {



AttachLayerJob::AttachLayerJob(const shared_ptr<Participant>& participant, const shared_ptr<Layer>& layer)
	: mParticipant(participant)
	, mLayer(layer)
{

}

AttachLayerJob::~AttachLayerJob()
{

}



void AttachLayerJob::process()
{
	NOT_IMPLEMENTED();
}

}}