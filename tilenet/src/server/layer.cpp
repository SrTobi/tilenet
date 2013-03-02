#include "includes.h"
#include "server/layer.h"
#include "server/display.h"
#include "server/server.h"

#include "service/netservice.h"
#include "service/participant.h"

#include "network/network.h"

namespace server {



AbstractLayer::AbstractLayer(nuint id, Display* display, const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below)
	:	mId(id),
		mAbove(above),
		mBelow(below),
		mDisplay(display)
{
}

AbstractLayer::~AbstractLayer()
{
}


AbstractLayer* AbstractLayer::above() const
{
	return mAbove.get();
}

AbstractLayer* AbstractLayer::below() const
{
	return mBelow.lock().get();
}

AbstractLayer* AbstractLayer::createLayer(const Point& position, const Rect& size, Level lvl)
{
	return createLayer(position, tn::StdRatio(), size, tn::StdRatio(), lvl);
}

AbstractLayer* AbstractLayer::createLayer(const Point& position, const Ratio& positionRatio,
											const Rect& size, const Ratio& sizeRatio, Level lvl)
{
	sync_guard lock(mPropertyMutex);

	using boost::make_shared;
	shared_ptr<AbstractLayer> layer;

	switch(lvl)
	{
	case Layer::Above:
		{
			layer = server::Layer::Create(position, positionRatio, size, sizeRatio, mDisplay, mAbove, shared_from_this());
			mAbove->mBelow = layer;
			mAbove = layer;
		}break;

	case Layer::Below:
		{
			layer = server::Layer::Create(position, positionRatio, size, sizeRatio, mDisplay, shared_from_this(), mBelow);
			shared_ptr<AbstractLayer> below(mBelow);

			below->mAbove = layer;
			mBelow = layer;
		}break;

	default:
		NOT_IMPL();
	}

	return layer.get();
}

Display* AbstractLayer::holdingDisplay() const
{
	return mDisplay;
}


tn::Display* AbstractLayer::display() const
{
	return mDisplay;
}


tn::Tilenet* AbstractLayer::tilenet() const
{
	return mDisplay->tilenet();
}

TilenetServer* AbstractLayer::server() const
{
	return mDisplay->server();
}


server::Layer* AbstractLayer::toLayer()
{
	server::Layer* l = dynamic_cast<server::Layer*>(this);

	assert(l);

	return l;
}


void AbstractLayer::setSelfAsAboveAndBelow()
{
	mBelow = mAbove = shared_from_this();
}

void AbstractLayer::resetBoth()
{
	mBelow.reset();
	mAbove.reset();
}



Layer::Layer(const Point& pos, const Ratio& positionRatio, const Rect& size,
				const Ratio& sizeRatio, Display* display,
				const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below)
	:	AbstractLayer(display->server()->requestLayerId(), display, above, below),
		mPosition(pos),
		mSize(size),
		mMarked(false),
		mSizeRatio(sizeRatio),
		mPositionRatio(positionRatio),
		mRefreshPoints(new std::vector<Point>())
{
	SLOG(Construct, L"Construct ServerLayer[id: " << id() << L"]...");
	refreshSize();
}

shared_ptr<Layer> Layer::Create(const Point& pos, const Ratio& positionRatio, const Rect& size,
								const Ratio& sizeRatio, Display* display,
								const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below)
{
	shared_ptr<Layer> layer(new Layer(pos, positionRatio, size, sizeRatio, display, above, below));
	
	layer->service()->start_LayerPropertiesTransmittion_broadcast(layer);
	return layer;
}

Layer::~Layer()
{
}

void Layer::setTile(const Point& pos, uint tile)
{
	Tile& t = mField[pos.x][pos.y];
	if(t.tile == tile)
		return;

	t.tile = tile;
	
	syncronize<decltype(mRefreshPoints)> rp(mRefreshPoints);
	rp->push_back(pos);

	if(!mMarked)
	{
		mMarked = true;
		service()->start_LayerTransmittion_broadcast(shared_from_derived_this());
	}

	//sf::Packet pack;
	//net::Pack<net::graphic::ToClient::PutTile>::Inject(pack, id(), pos, tile, t.color);
	//mDisplay->send(pack);
}

void Layer::put(const Point& pos, uint tile, uint color)
{
	Tile& t = mField[pos.x][pos.y];
	if(t.tile == tile && t.color == color)
		return;

	t.tile = tile;
	t.color = color;

	syncronize<decltype(mRefreshPoints)> rp(mRefreshPoints);
	rp->push_back(pos);
	
	if(!mMarked)
	{
		mMarked = true;
		service()->start_LayerTransmittion_broadcast(shared_from_derived_this());
	}

	//sf::Packet pack;
	//net::Pack<net::graphic::ToClient::PutTile>::Inject(pack, id(), pos, tile, color);
	//mDisplay->send(pack);
}

uint Layer::getTile(const Point& pos) const
{
	return mField[pos.x][pos.y].tile;
}

uint Layer::getColor(const Point& pos) const
{
	return mField[pos.x][pos.y].color;
}

void Layer::setPosition(const Point& pos)
{
	{
		sync_guard lock(mPropertyMutex);
		mPosition = pos;
	}

	service()->start_LayerPropertiesTransmittion_broadcast(shared_from_derived_this());
}

void Layer::setSize(const Rect& size)
{
	{
		sync_guard lock(mPropertyMutex);
		mSize = size;
		refreshSize();
	}
	
	service()->start_LayerPropertiesTransmittion_broadcast(shared_from_derived_this());
}


Rect Layer::size() const 
{
	return mSize;
}

Point Layer::position() const
{
	return mPosition;
}


shared_ptr<service::NetService> Layer::service() const
{
	return server()->service();
}



const Layer::TileField& Layer::tileField() const
{
	return mField;
}


/*
void Layer::broadcastProperties() const
{
	std::vector< shared_ptr<service::Participant> > participants;
	mDisplay->getParticipants(participants);
	

	for(auto it = participants.begin();
		it != participants.end();
		++it)
	{
		if(*it)
		{
			net::send((*it)->socket(), pack);
		}
	}

}


void Layer::sendProperties(const shared_ptr<service::Participant>& participant) const
{
	if(!participant)
		return;

	net::OBuffer pack;
	gatherProperties(pack);

	net::send(participant->socket(), pack);
}
*/

void Layer::gatherProperties(net::OBuffer& target) const
{
	sync_guard lock(mPropertyMutex);


	net::Protocol<net::msg::ToClient::SendLayerProperties>::Inject(	target,
																	id(),
																	mPosition, mPositionRatio,
																	mSize, mSizeRatio,
																	//TODO: setzbar machen
																	0xFF,
																	above()->id());
}

std::vector<Point>* Layer::swapBuffer(std::vector<Point>* vec)
{
	syncronize<decltype(mRefreshPoints)> rp(mRefreshPoints);
	
	mMarked = false;
	std::swap(rp.ref(), vec);
	return vec;
}


void Layer::refreshSize()
{
	mField.resize(mSize.w);
	for(auto it = mField.begin();
		it != mField.end();
		++it)
	{
		it->resize(mSize.h, Tile());
	}
}

bool Layer::isEnd() const
{
	return false;
}


LayerGuard::LayerGuard(Display* display)
	: AbstractLayer(0, display, shared_ptr<AbstractLayer>(), weak_ptr<AbstractLayer>())
{
}

LayerGuard::~LayerGuard()
{}


void LayerGuard::init()
{
	setSelfAsAboveAndBelow();
}

void LayerGuard::reset()
{
	resetBoth();
}


void LayerGuard::setTile( const Point& pos, uint tile )
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

void LayerGuard::put( const Point& pos, uint tile, uint color /*= 0xFFFFFFFF */ )
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

uint LayerGuard::getTile( const Point& pos ) const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

uint LayerGuard::getColor( const Point& pos ) const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

void LayerGuard::setPosition( const Point& pos )
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

void LayerGuard::setSize( const Rect& size )
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

Rect LayerGuard::size() const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

Point LayerGuard::position() const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

Ratio LayerGuard::sizeRatio() const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

Ratio LayerGuard::positionRatio() const
{
	throw tn::TnException(L"This class does not support the wanted operation!");
}

bool LayerGuard::isEnd() const
{
	return true;
}


}	//End Namespace
