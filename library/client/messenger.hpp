#pragma once
#ifndef _MESSENGER_H
#define _MESSENGER_H

#include <list>
#include <chrono>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "settings.hpp"

namespace client {


class Messenger
{
	typedef std::chrono::system_clock clock;
	typedef std::chrono::time_point<clock> time_point;
	typedef clock::duration duration;

	struct Entry
	{
		Entry(const std::wstring& text, sf::Font& font)
			: myText(lexical_convert<std::string>(text), font)
			, mBirthtime(clock::now())
		{
		}

		inline duration livetime() const { return clock::now() - mBirthtime; }

		sf::Text	myText;
		time_point	mBirthtime;
	};
public:
	Messenger(unsigned int maxEntries, duration entryLiveTime, unsigned int textSize);
	~Messenger();

	void render(sf::RenderTarget& g);

	void add(const std::wstring& text, sf::Color color = sf::Color::White);

private:
	std::wstring					mNextLine;

	std::list<shared_ptr<Entry> >	mEntries;
	duration						mEntryLiveTime;
	unsigned int					mMaxEntries;
	unsigned int					mTextSize;

	sf::Font						mFont;
};


}




#endif