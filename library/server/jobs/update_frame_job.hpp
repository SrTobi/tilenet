#pragma once
#ifndef _UPDATE_LAYER_JOB_HPP
#define _UPDATE_LAYER_JON_HPP


#include <vector>
#include "settings.hpp"

#include "service_job.hpp"
#include "network/protocol.hpp"


namespace srv {

class Layer;
class FrameLayer;

namespace job {


	class UpdateFrameJob
		: public ServiceJob
	{
		typedef proto::curv::PView PView;
	public:
		UpdateFrameJob(const shared_ptr<FrameLayer>& frame, std::unordered_map<shared_ptr<Layer>, std::pair<PView, unsigned short>>&& sublayers);
		~UpdateFrameJob();


		virtual OVERRIDE void process();

	private:
		shared_ptr<FrameLayer> mFrame;
		std::unordered_map<shared_ptr<Layer>, std::pair<PView, unsigned short>> mSublayers;
	};






}}







#endif