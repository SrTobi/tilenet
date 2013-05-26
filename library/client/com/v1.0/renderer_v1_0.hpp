#pragma once
#ifndef _RENDERER_V1_0_HPP
#define _RENDERER_V1_0_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include <unordered_map>

#include "settings.hpp"
#include "client/render_interface.hpp"
#include "network/v1.0/protocol_tile.hpp"


namespace client{

class ClientWindow;
class TileManager;

namespace com {
namespace v1_0 {



class Renderer
	: public RenderInterface
{
	class Layer;
	class FrameLayer;
	class RenderLayer;
public:
	Renderer(const shared_ptr<ClientWindow>& window, const shared_ptr<TileManager>& manager);
	~Renderer();

	virtual OVERRIDE void render(sf::RenderTarget& target);

	void setTopLayer(TNID id);
	void defineLayer(TNID id, Rect size, Ratio r);
	void putTile(TNID layerid, Point pos, const net::PTile& tile);

private:
	shared_ptr<Layer> layer(TNID id) const;

private:
	shared_ptr<TileManager> mTileManager;
	weak_ptr<sf::RenderTarget> mTarget;
	weak_ptr<ClientWindow> mWindow;

	std::unordered_map<TNID, shared_ptr<Layer>> mIdToLayerMapping;
	TNID mTopLayerId;

	sf::Color mBGCOlor;
};




}}}


#endif