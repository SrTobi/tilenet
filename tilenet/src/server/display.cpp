#include "includes.h"
#include "server/display.h"
#include "server/layer.h"
#include "server/server.h"
#include "service/participant.h"

namespace server {

Display::Display(TilenetServer* server)
	: mServer(server)
{
	SLOG(Construct, L"Construct display");

	mLayers = make_shared<LayerGuard>(this);
	mLayers->init();
}


Display::~Display()
{
	mLayers->reset();
}


AbstractLayer* Display::createLayer(const Point& position, const Rect& size, Level lvl)
{
	return createLayer(position, tn::StdRatio(), size, tn::StdRatio(), lvl);
}

AbstractLayer* Display::createLayer(const Point& position, const tn::Ratio& positionRatio,
								const Rect& size, const tn::Ratio& sizeRatio, Level lvl)
{
	Layer::Level level;

	switch(lvl)
	{
	case tn::Display::TopMost:
		level = LayerGuard::Below;
		break;
	case tn::Display::BottomMost:
		level = LayerGuard::Above;
		break;

	default:
		NOT_IMPL();
	};


	return mLayers->createLayer(position, positionRatio, size, sizeRatio, level);
}

AbstractLayer* Display::topLayer() const
{
	return mLayers->below();
}

AbstractLayer* Display::bottomLayer() const
{
	return mLayers->above();
}


void Display::addParticipant(const shared_ptr<service::Participant>& p)
{
	syncronize<decltype(mParticipants)> participants(mParticipants);
	participants->push_back(p);
}

void Display::getParticipants(std::vector< shared_ptr<service::Participant> >& list)
{
	syncronize<decltype(mParticipants)> participants(mParticipants);

	list.reserve(participants->size());

	for(auto it = participants->begin();
		it != participants->end();
		++it)
	{
		if(it->expired())
		{
			auto toDel = it;
			--it;
			participants->erase(toDel);
		}else{
			list.push_back(it->lock());
		}
	}
}

tn::Tilenet* Display::tilenet() const
{
	return mServer;
}

TilenetServer* Display::server() const
{
	return mServer;
}

shared_ptr<service::NetService> Display::service() const
{
	return server()->service();
}

AbstractLayer* Display::end() const
{
	return mLayers.get();
}


}