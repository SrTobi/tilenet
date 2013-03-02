#pragma once
#ifndef _LAYER_H
#define _LAYER_H

#include "settings.h"

//#include "service/netservice.h"

#include "utils/signal.h"


namespace service {
	class NetService;
	class Participant;
}

namespace net {
	class OBuffer;
}


namespace server {


class TilenetServer;
class Layer;
class Display;


class AbstractLayer
	:	public tn::Layer,
		public boost::enable_shared_from_this<AbstractLayer>
{
public:
	AbstractLayer(nuint id, Display* display, const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below);
	virtual ~AbstractLayer();

	inline nuint id() const { return mId; }

	//@Override
	AbstractLayer* above() const;

	//@Override
	AbstractLayer* below() const;

	//@Override
	AbstractLayer* createLayer(const Point& position, const Rect& size, Level lvl = Above);

	//@Override
	AbstractLayer* createLayer(const Point& position, const Ratio& positionRatio,
								const Rect& size, const Ratio& sizeRatio, Level lvl = Above);

	//@Override
	tn::Display* display() const;

	// @Override
	tn::Tilenet* tilenet() const;

	server::Layer*	toLayer();
	TilenetServer*	server() const;
	Display*		holdingDisplay() const;

protected:
	void setSelfAsAboveAndBelow();
	void resetBoth();
	mutable boost::mutex mPropertyMutex;

private:
	Display* mDisplay;

	shared_ptr<AbstractLayer> mAbove;
	weak_ptr<AbstractLayer> mBelow;
	
	const nuint mId;
};





class Layer
	:	public AbstractLayer,
		public utils::enable_shared_for_derived<Layer, AbstractLayer>
{
	friend class Display;
public:
	struct Tile
	{
		Tile():tile(0),color(0) {}
		nuint tile;
		nuint color;
	};
	typedef std::vector< std::vector<Tile> > TileField;
public:
	virtual ~Layer();
	
	//@Override
	void setTile(const Point& pos, uint tile);
	
	//@Override
	void put(const Point& pos, uint tile, uint color = 0xFFFF);
	
	//@Override
	uint getTile(const Point& pos) const;
	
	//@Override
	uint getColor(const Point& pos) const;
	
	//@Override
	void setPosition(const Point& pos);

	//@Override
	void setSize(const Rect& size);
	
	//@Override
	Rect size() const;

	//@Override
	Point position() const;

	//@Override
	Ratio sizeRatio() const	{ return mSizeRatio; }

	//@Override
	Ratio positionRatio() const	{ return mPositionRatio; }
	

	shared_ptr<service::NetService>	service() const;
	void gatherProperties(net::OBuffer& target) const;

	const TileField& tileField() const;

	std::vector<Point>* swapBuffer(std::vector<Point>* vec);

	static shared_ptr<Layer> Create(const Point& pos, const Ratio& positionRatio, const Rect& size,
									const Ratio& sizeRatio, Display* display,
									const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below);

	//@Override
	bool isEnd() const;

private:
	
	Layer(const Point& pos, const Ratio& positionRatio, const Rect& size,
				const Ratio& sizeRatio, Display* display,
				const shared_ptr<AbstractLayer>& above, const weak_ptr<AbstractLayer>& below);

	void refreshSize();

private:

	TileField mField;

	Rect mSize;
	Point mPosition;

	Ratio mSizeRatio;
	Ratio mPositionRatio;

	bool								mMarked;
	Syncronized< std::vector<Point>* >	mRefreshPoints;
};



class LayerGuard
	: public AbstractLayer
{
public:
	LayerGuard(Display* display);
	virtual ~LayerGuard();

	void init();
	void reset();

	//@Override
	virtual void setTile( const Point& pos, uint tile );

	//@Override
	virtual void put( const Point& pos, uint tile, uint color /* = 0xFFFFFFFF */ );

	//@Override
	virtual uint getTile( const Point& pos ) const;

	//@Override
	virtual uint getColor( const Point& pos ) const;

	//@Override
	virtual void setPosition( const Point& pos );

	//@Override
	virtual void setSize( const Rect& size );

	//@Override
	virtual Rect size() const;

	//@Override
	virtual Point position() const;

	//@Override
	virtual Ratio sizeRatio() const;

	//@Override
	virtual Ratio positionRatio() const;

	//@Override
	bool isEnd() const;

};




}


#endif
