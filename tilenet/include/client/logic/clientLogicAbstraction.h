#pragma once
#ifndef _CLIENTABSTRACTION_H
#define _CLIENTABSTRACTION_H

#include "settings.h"
#include "network/ibuffer.h"

namespace client {

class ClientApp;

class ClientLogicAbstraction
{
public:
	class Factory
	{
	public:
		virtual uint version() const = 0;
		virtual shared_ptr<ClientLogicAbstraction> build(net::IBuffer::Map& ini, const shared_ptr<ClientApp>& app) const = 0;
	};
	template<typename T>
	class TemplateFactory
		: public Factory
	{
	public:
		TemplateFactory(uint version)
			: mVersion(version)
		{}

		uint version() const
		{
			return mVersion;
		}

		shared_ptr<ClientLogicAbstraction> build(net::IBuffer::Map& ini, const shared_ptr<ClientApp>& app) const
		{
			try
			{
				return boost::make_shared<T>(ini, app);
			}
			catch (tn::TnException&)
			{
				CLOG(Error, L"Failed to create client logic!")
				return shared_ptr<ClientLogicAbstraction>();
			}
		}

	private:
		const uint mVersion;
	};


public:
	//ClientLogicAbstraction(net::IBuffer::Map& ini, ClientApp* app);

	
	virtual void onRender() = 0;
	virtual void onNetworkPackage(net::IBuffer& package) = 0;
	virtual void onClose() = 0;

	virtual void onKeyEvent(sf::Event::KeyEvent& e, bool pressed) = 0;
	virtual std::pair<Point, Point> getBounds() = 0;

};




}



#endif
