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

class RenderLayer;


class Renderer
	: public RenderInterface
{
public:
	Renderer(const shared_ptr<ClientWindow>& window);
	~Renderer();


	virtual OVERRIDE void init(const shared_ptr<sf::RenderTarget>& target);
	virtual OVERRIDE void render();

	void setTopLayer(TNID id);

private:
	weak_ptr<sf::RenderTarget> mTarget;
	weak_ptr<ClientWindow> mWindow;

	std::unordered_map<TNID, shared_ptr<RenderLayer>> mLayerList;
	TNID mTopLayerId;
};




}}}


#endif