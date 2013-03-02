#include "includes.h"
#include "client/logic/v1/clientLayer.h"
#include "client/logic/v1/clientLogic.h"


namespace client {
namespace v1 {


ClientLayer::ClientLayer(nuint id, const Point& position, const Rect& size, const shared_ptr<ClientLogic>& logic)
	: mPosition(position), mId(id), mLogic(logic)
{
	CLOG(Construct, L"Create Layer[" << position << L"," << size << "]");
	mField.resize(size.w);
	setPosition(position);
	setSize(size);
}

ClientLayer::~ClientLayer()
{
}

void ClientLayer::updateProperties(const Point& position, const Rect& size)
{
	mPosition = position;
	setSize(size);
}

void ClientLayer::setPosition(const Point& pos)
{
	mPosition = pos;	
	setSize(size());
}

sf::Sprite* ClientLayer::tile(const Point& pos)
{
	return &(mField[pos.x][pos.y]);
}


void ClientLayer::render(sf::RenderTarget& target)
{
	int y = 0;
	int x = 0;

	for(auto xit = mField.begin();
		xit != mField.end();
		++xit)
	{
		y = 0;
		for(auto yit = xit->begin();
			yit != xit->end();
			++yit)
		{
		/*	auto size = yit->getTexture()->getSize();
			auto rect = yit->getTextureRect();
			auto col = yit->getColor();;*/
		//	std::cout << "(" << x << ", " << y << ") " <<  size.x << ", " << size.y << ": " << rect.left << ", " << rect.top << "[" << (int)col.r << ", " << (int)col.g << ", " << (int)col.b << ", " << (int)col.a << "]" << std::endl;
			target.draw(*yit);
			++y;
		}
		++x;
	}
}

void ClientLayer::setSize(const Rect& size)
{
	Rect tsize = mLogic.lock()->tileSize();
	Point pos = mPosition * tsize;

	mField.resize(size.w);
	for(auto it = mField.begin();
		it != mField.end();
		++it)
	{
		int d = 0;
		it->resize(size.y);
		for(auto it2 = it->begin();
			it2 != it->end();
			++it2, d += tsize.y)
		{
			it2->setPosition((float)pos.x, (float)pos.y + d);
		}

		pos.x += tsize.x;
	}
}

}}
