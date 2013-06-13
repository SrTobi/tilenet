#pragma once
#ifndef _UPDATE_LAYER_JOB_HPP
#define _UPDATE_LAYER_JON_HPP



#include "settings.hpp"

#include "service_job.hpp"
#include "network/v1.0/protocol_tile.hpp"


namespace srv {

class TileLayer;

namespace job {


	class UpdateLayerJob
		: public ServiceJob
	{
	public:
		UpdateLayerJob(const shared_ptr<TileLayer>& layer, std::vector<net::PTile>&& tiles, std::vector<bool>&& toupdate);
		~UpdateLayerJob();


		virtual OVERRIDE void process();

	private:
		shared_ptr<TileLayer> mLayer;
		std::vector<net::PTile> mTiles;
		std::vector<bool> mToUpdate;
	};






}}







#endif