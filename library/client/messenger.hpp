#pragma once
#ifndef _MESSENGER_H
#define _MESSENGER_H

#include <list>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "settings.hpp"

namespace client {


class Messenger
{
	struct Entry
	{
		Entry(const std::wstring& text, sf::Font& font)
			: myText(lexical_convert<std::string>(text), font)
		{
		}

		inline unsigned int livetime() const { return myLiveTime.getElapsedTime().asMilliseconds(); }

		sf::Text	myText;
		sf::Clock	myLiveTime;
	};
public:
	Messenger(unsigned int maxEntries, unsigned int entryLiveTime, unsigned int textSize);
	~Messenger();

	void render(sf::RenderTarget& g);

	void add(const std::wstring& text, sf::Color color = sf::Color::White);

private:
	std::wstring					mNextLine;

	std::list<shared_ptr<Entry> >	mEntries;
	unsigned int					mEntryLiveTime;
	unsigned int					mMaxEntries;
	unsigned int					mTextSize;

	sf::Font						mFont;
};


}




#endif