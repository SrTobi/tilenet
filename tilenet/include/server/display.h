#pragma once
#ifndef _DISPLAY_H
#define _DISPLAY_H


#include "settings.h"

#include "server/layer.h"
//#include "service/participant.h"

namespace service{
	class NetService;
	class Participant;
}

namespace server {
class TilenetServer;


class Display
	:	public tn::Display,
		public boost::enable_shared_from_this<Display>
{
	friend class Layer;
public:
	Display(TilenetServer* server);
	~Display();
	
	//@Overrride
	AbstractLayer* createLayer(const Point& position, const Rect& size, Level lvl = TopMost);
	
	//@Overrride
	AbstractLayer* createLayer(const Point& position, const tn::Ratio& positionRatio,
							const Rect& size, const tn::Ratio& sizeRatio, Level lvl = TopMost);

	//@Overrride
	AbstractLayer* topLayer() const;

	//@Overrride
	AbstractLayer* bottomLayer() const;

	//@Override
	tn::Tilenet* tilenet() const;

	//@Override
	AbstractLayer* end() const;
	
	TilenetServer* server() const;
	
	shared_ptr<service::NetService> service() const;


	void addParticipant(const shared_ptr<service::Participant>& p);

	void getParticipants(std::vector< shared_ptr<service::Participant> >& list);

private:

	TilenetServer* mServer;

	Syncronized<std::list< weak_ptr<service::Participant> > > mParticipants;

	shared_ptr<LayerGuard> mLayers;
};

}



#endif
