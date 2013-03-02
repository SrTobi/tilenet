#pragma once
#ifndef _CLIENT_V1_H
#define _CLIENT_V1_H

#include "settings.h"
#include "client/logic/clientLogicAbstraction.h"


namespace client {
namespace v1 {


class ClientLayer;


class ClientLogic
	:	public ClientLogicAbstraction,
		public boost::enable_shared_from_this<ClientLogic>
{
public:
	struct PTile
	{
		PTile(nuint id, sf::Sprite* t, nuint c)
			: tileId(id), target(t), color(c)
			{}
		nuint tileId;
		sf::Sprite* target;
		nuint color;
	};
public:
	ClientLogic(net::IBuffer::Map& ini, const shared_ptr<ClientApp>& app);
	~ClientLogic();
	
	void loadImageFile(const fs::path& path, const wstring& fname);
	void loadPackage(const fs::path& path);
	sf::Sprite* getTileSprite(nuint id);
	void answerHandshake();

	//@Override
	void onRender();

	//@Override
	void onNetworkPackage(net::IBuffer& package);

	//@Override
	void onClose();

	//@Override
	void onKeyEvent(sf::Event::KeyEvent& e, bool pressed);

	//@Override
	std::pair<Point, Point> getBounds();

	
	inline const Rect& tileSize() const { return mTileSize; }
	inline socket_ptr socket() const { return mSocket; }
private:

	bool handleCollectionPackage(net::IBuffer& buffer);
	bool handleCollectionAswerPackage(net::IBuffer::Map& content);
	void handleLayerPropertyTransmission(net::IBuffer& buffer);
	void handleLayerContentUpdate(net::IBuffer& buffer);
	void handleFullLayerContent( net::IBuffer& package );
	void handleRequestedTileDefinition(net::IBuffer& buffer);

	void addContentTile(const shared_ptr<ClientLayer>& layer, const Point& pos, nuint tileId, nuint color);

private:
	shared_ptr<ClientApp>	mApp;
	socket_ptr				mSocket;
	sf::RenderWindow&		mGraphics;

	Rect mTileSize;

	std::list<shared_ptr<ClientLayer> >	mLayerStack;

	std::map<nstring, sf::Sprite>	mTileMap;
	std::set<nuint>					mRequestedTiles;
	std::vector<sf::Sprite*>		mTileList;
	std::vector<sf::Texture>		mTextures;
	std::queue<PTile>				mPendingTiles;
};








}}

#endif
