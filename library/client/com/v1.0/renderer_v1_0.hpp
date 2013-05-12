#pragma once
#ifndef _RENDERER_V1_0_HPP
#define _RENDERER_V1_0_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include <unordered_map>

#include "settings.hpp"
#include "client/render_interface.hpp"



namespace client{

class ClientWindow;

namespace com {
namespace v1_0 {



class Renderer
	: public RenderInterface
{
	class Layer;
	class FrameLayer;
	class RenderLayer;
public:
	Renderer(const shared_ptr<ClientWindow>& window);
	~Renderer();


	virtual OVERRIDE void render(sf::RenderTarget& target);

	void setTopLayer(TNID id);

private:
	shared_ptr<Layer> layer(TNID id) const;

private:
	weak_ptr<sf::RenderTarget> mTarget;
	weak_ptr<ClientWindow> mWindow;

	std::unordered_map<TNID, shared_ptr<Layer>> mIdToLayerMapping;
	TNID mTopLayerId;

	sf::Color mBGCOlor;
};




}}}


#endif