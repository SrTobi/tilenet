#include "includes.h"
#include "client/logic/v1/clientLogic.h"
#include "client/logic/v1/clientLayer.h"
#include "client/client.h"





namespace client {
namespace v1 {






ClientLogic::ClientLogic(net::IBuffer::Map& ini, const shared_ptr<ClientApp>& app)
	:	mApp(app),
		mGraphics(app->graphics()),
		mSocket(app->socket())
{
	mTileList.resize(1024, nullptr);
	mTileSize = Rect(8,12);

	answerHandshake();
	
	CLOG(Info, L"Load std resources...");
	loadImageFile(L"tilesets/test/", L"img.txt");
}


ClientLogic::~ClientLogic()
{


}


void ClientLogic::loadImageFile(const fs::path& path, const wstring& fname)
{
	fs::path filepath(path/fname);
	
	CLOG(Info, L"Load file[" << filepath.wstring() << L"]");
	std::wfstream file(filepath.string());

	if(!file.is_open())
	{
		CLOG(Error, L"Not able to load file!");
		throw tn::TnException(L"Not able to load file!");
	}

	wstring line;
	std::getline(file, line);

	CLOG(Info, L"Load texture[" << (path / line).wstring() << L"]");
	mTextures.push_back(sf::Texture());
	sf::Texture& tex = mTextures.back();
	if(!tex.loadFromFile((path / line).string()))
	{
		CLOG(Error, L"Not able to load texture[" << (path / line).wstring()  << L"]");
	}

	while(!file.eof())
	{
		wchar ch;
		Point pos;
		file >> ch >> pos >> ch >> ch;
		wstring name;
		std::getline(file, name);
		mTileMap.insert(std::make_pair(name, sf::Sprite(tex, sf::IntRect(mTileSize.w * pos.x, mTileSize.h * pos.y,
															//		mTileSize.w * (pos.x+1), mTileSize.h * (pos.y+1)))));
															mTileSize.w, mTileSize.h))));
		if(name==L"xxx")
			break;
	}
}

void ClientLogic::loadPackage(const fs::path& path)
{

}

sf::Sprite* ClientLogic::getTileSprite(nuint id)
{
	if(id >= mTileList.size())
		return nullptr;

	return mTileList[id];
}


void ClientLogic::answerHandshake()
{
	net::OBuffer::Map answ;

	answ["subject"] = L"handshake_back";
	answ["password"] = L"";

	net::send(socket(), answ);
}


void ClientLogic::onRender()
{
	for(auto it = mLayerStack.begin();
		it != mLayerStack.end();
		++it)
	{
		(*it)->render(mGraphics);
	}

}

void ClientLogic::onNetworkPackage(net::IBuffer& package)
{
	CLOG(Info, L"Incoming package with id " << package.id());
	switch(package.id())
	{
	case net::msg::Collection:
		handleCollectionPackage(package);
		break;
	case net::msg::ToClient::SendLayerProperties:
		handleLayerPropertyTransmission(package);
		break;

	case net::msg::ToClient::SendUpdatedLayerContent:
		handleLayerContentUpdate(package);
		break;

	case net::msg::ToClient::SendFullLayerContent:
		handleFullLayerContent(package);
		break;
	
	case net::msg::ToClient::SendTileDefinition:
		handleRequestedTileDefinition(package);
		break;

	default:
		// Es ist was ungueltiges, also gleich disconnecten
		CLOG(Warning, L"Package with unknown package id arrived!");
		break;
	}
}

void ClientLogic::onClose()
{

}

std::pair<Point, Point> ClientLogic::getBounds()
{
	Point topLeft(10000, 10000);
	Point bottomRight(-10000, -10000);

	for(auto it = mLayerStack.begin();
		it != mLayerStack.end();
		++it)
	{
		Point tl = (*it)->position();
		Point br = tl + (*it)->size();

		topLeft.x = std::min(topLeft.x, tl.x);
		topLeft.y = std::min(topLeft.y, tl.y);
		bottomRight.x = std::max(bottomRight.x, br.x);
		bottomRight.y = std::max(bottomRight.y, br.y);
	}

	return std::make_pair(topLeft * mTileSize, bottomRight * mTileSize);
}




bool ClientLogic::handleCollectionPackage(net::IBuffer& buffer)
{
	net::Protocol<net::msg::Collection>	protocol(buffer);

	wstring subject = protocol.collection["subject"];

	if(subject == L"answer")
	{
		return handleCollectionAswerPackage(protocol.collection);
	}else{
		CLOG(Error, "Unknown command from server!");
	}


	return true;
}


bool ClientLogic::handleCollectionAswerPackage(net::IBuffer::Map& content)
{
	auto& answer = content["answer"];


	if(answer == L"access_granted")
	{
		CLOG(Info, L"Access to server granted!")
	}else if(answer == L"invalid_password")
	{
		CLOG(Warning, L"Invalid password was used!");
		return false;

	}else{
		CLOG(Error, L"Invalid answer from server");
		return false;
	}

	return true;
}

void ClientLogic::handleLayerPropertyTransmission(net::IBuffer& buffer)
{
	net::Protocol<net::msg::ToClient::SendLayerProperties> protocol(buffer);

	auto overlyingLayer = mLayerStack.end();

	for(auto it = mLayerStack.begin();
		it != mLayerStack.end();
		++it)
	{
		if((*it)->id() == protocol.layerId)
		{
			//CLOG(Error, L"Can not create layer twice (id: " << protocol.layerId <<L")!");
			shared_ptr<ClientLayer> layer = *it;

			layer->updateProperties(protocol.position, protocol.size);
			mApp->calculateViewport();

			return;
		}

		if((*it)->id() == protocol.overlyingLayer)
		{
			overlyingLayer = it;
		}
	}

	if(overlyingLayer == mLayerStack.end() && protocol.overlyingLayer)
	{
		CLOG(Error, L"Can not create layer[" << protocol.layerId << L"] under layer[" << protocol.overlyingLayer << L"] because second one could not be found!");
	}else{
		shared_ptr<ClientLayer> layer  = make_shared<ClientLayer>(	protocol.layerId,
			protocol.position,
			protocol.size,
			shared_from_this());
			
		mLayerStack.insert(overlyingLayer, layer);
		mApp->calculateViewport();
	}

}


void RenderTile(sf::Sprite* target, sf::Sprite* src, nuint color)
{
//	std::cout << "draw[" << target->getPosition().x << ", " << target->getPosition().y << "]:" << color << std::endl;
	target->setTexture(*src->getTexture());
	target->setTextureRect(src->getTextureRect());
	target->setColor(sf::Color((color >> 16)&0xFF, (color >> 8)&0xFF, color&0xFF, (color >> 24)& 0xFF));
}

void ClientLogic::handleLayerContentUpdate(net::IBuffer& buffer)
{
	net::Protocol<net::msg::ToClient::SendUpdatedLayerContent> protocol(buffer);

	shared_ptr<ClientLayer> layer;

	for(auto it = mLayerStack.begin();
		it != mLayerStack.end();
		++it)
	{
		if((*it)->id() == protocol.layerId)
		{
			layer = (*it);
			break;
		}
	}

	if(!layer)
	{
		CLOG(Error, L"Layer with specified layer id[" << protocol.layerId << L"] does not exist!");
		return;
	}

	Point pos;
	Rect size = layer->size();

	for(nuint i = 0; i < protocol.count; ++i)
	{
		nuint tileId, offset, tileColor;
		buffer >> offset >> tileId  >> tileColor;

		// Position berechnen
		nuint distanceToEndOfLine = size.w - pos.x;
		if(distanceToEndOfLine <= offset)
		{
			pos.y++;
			offset -= distanceToEndOfLine;

			pos.y += offset / size.w;
			pos.x = offset % size.w;

		}else{
			pos.x += offset;
		}

		if(pos.x < 0 || pos.x >= size.w || pos.y < 0 || pos.y >= size.h)
		{
			CLOG(Error, L"Error in layer-offset!");
			return;
		}
		addContentTile(layer, pos, tileId, tileColor);
	}
}



void ClientLogic::handleFullLayerContent( net::IBuffer& package )
{
	net::Protocol<net::msg::ToClient::SendFullLayerContent> protocol(package);
	shared_ptr<ClientLayer> layer;

	for(auto it = mLayerStack.begin();
		it != mLayerStack.end();
		++it)
	{
		if((*it)->id() == protocol.layerId)
		{
			layer = (*it);
			break;
		}
	}

	if(!layer)
	{
		CLOG(Error, L"Layer with specified layer id[" << protocol.layerId << L"] does not exist!");
		return;
	}

	Rect size =	layer->size();

	if(protocol.count != size.area()) {
		CLOG(Error, L"Size of layer is inconsistence!");
		return;
	}
	Point pos;

	for(pos.x = 0; pos.x < size.w; ++pos.x)
	{
		for(pos.y = 0; pos.y < size.h; ++pos.y)
		{
			nuint tileId, color;
			package >> tileId >> color;
			addContentTile(layer, pos, tileId, color);
		}
	}
}


void ClientLogic::handleRequestedTileDefinition(net::IBuffer& buffer)
{
	net::Protocol<net::msg::ToClient::SendTileDefinition> protocol(buffer);

	if(protocol.tileId >= mTileList.size())
		mTileList.resize(protocol.tileId+1);

	if(!mTileList[protocol.tileId])
	{
		mTileList[protocol.tileId] = &(mTileMap[protocol.tileName]);
	}

	sf::Sprite* sp = nullptr;
	PTile* ptile;
	while(mPendingTiles.size() && (sp = getTileSprite((ptile = &mPendingTiles.front())->tileId)))
	{
		RenderTile(ptile->target, sp, ptile->color);
		mPendingTiles.pop();
	}
}



void ClientLogic::addContentTile(const shared_ptr<ClientLayer>& layer, const Point& pos, nuint tileId, nuint color )
{
	sf::Sprite* sp = getTileSprite(tileId);
	sf::Sprite* target = layer->tile(pos);

	if(mPendingTiles.size() || !sp)
	{
		// Beim Server anfragen
		if(mRequestedTiles.find(tileId) == mRequestedTiles.end())
		{
			mRequestedTiles.insert(tileId);

			net::OBuffer request(net::msg::ToServer::RequestTileDef);
			net::Protocol<net::msg::ToServer::RequestTileDef>::Inject(request, tileId);
			net::send(socket(), request);

			//sf::Packet pack;
			//net::Pack<net::graphic::ToServer::RequestTileDef>::Inject(pack, p->tileId);
			//send(pack);
		}
		mPendingTiles.push(PTile(tileId, target, color));
	}else{
		// Rendern
		RenderTile(target, sp, color);
	}
}

void ClientLogic::onKeyEvent(sf::Event::KeyEvent& e, bool pressed)
{
	using tn::KeyEvent;
	using sf::Keyboard;

	byte modifier = pressed? KeyEvent::Pressed : 0;
	modifier |= e.system? KeyEvent::SystemKey : 0;
	modifier |= e.alt? KeyEvent::AltKey : 0;
	modifier |= e.shift? KeyEvent::ShiftKey : 0;
	modifier |= e.control? KeyEvent::ControlKey : 0;

	wchar_t code = 0;

	if(e.code >= Keyboard::A && e.code <= Keyboard::Z)
	{
		code = 'a' + e.code;
	}else if(e.code >= Keyboard::Num0 && e.code < Keyboard::KeyCount)
	{
		modifier |= KeyEvent::SpecialKey;
		code = e.code - Keyboard::Num0;
	}else{
		return;
	}

	net::OBuffer msg(net::msg::ToServer::SendKeyEvent);
	net::Protocol<net::msg::ToServer::SendKeyEvent>::Inject(msg, modifier, code);
	net::send(socket(), msg);
}



}}
