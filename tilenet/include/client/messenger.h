#pragma once
#ifndef _MESSANGER_H
#define _MESSANGER_H


#include "settings.h"

namespace client {


class Messenger
	: public std::wstreambuf
{
	struct Entry
	{
		Entry(const wstring& text)
			:	myText(sf::String(text))
		{
		}

		inline uint livetime() const { return myLiveTime.getElapsedTime().asMilliseconds(); }

		sf::Text	myText;
		sf::Clock	myLiveTime;
	};
public:
	Messenger(sf::RenderWindow& graphics, std::wostream& stream, uint maxEntries, uint entryLiveTime, uint textSize);
	~Messenger();

	void render();

	void add(const wstring& text, sf::Color color = sf::Color::White);

private:
	//@Override
	int_type overflow(int_type c = traits_type::eof() );

private:

	std::wstreambuf*	mOldSB;
	std::wostream&		mStream;
	wstring				mNextLine;

	std::list<shared_ptr<Entry> >	mEntries;
	uint							mEntryLiveTime;
	uint							mMaxEntries;
	uint							mTextSize;

	sf::RenderWindow&				mGraphics;
};


}




#endif