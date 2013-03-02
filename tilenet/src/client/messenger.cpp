#include "includes.h"
#include "client/messenger.h"



namespace client {


	
Messenger::Messenger(sf::RenderWindow& graphics, std::wostream& stream, uint maxEntries, uint entryLiveTime, uint textSize)
	:	mEntryLiveTime(entryLiveTime),
		mMaxEntries(maxEntries),
		mTextSize(textSize),
		mGraphics(graphics),
		mStream(stream),
		mOldSB(stream.rdbuf())
{
	mStream.rdbuf(this);
}

Messenger::~Messenger()
{
}

void Messenger::render()
{
	// wenn zu viele Einträge vorhanden sind, einige loeschen
	while(mEntries.size() > mMaxEntries)
		mEntries.pop_front();

	// Alle loeschen, die schon zu lange leben
	while(mEntries.size() && mEntries.front()->livetime() > mEntryLiveTime)
		mEntries.pop_front();

	// rendern
	float ypos = 5.0f;
	for(auto it = mEntries.begin();
		it != mEntries.end();
		++it)
	{
		sf::Text& text = (*it)->myText;

		text.setPosition(5.0f, ypos);

		mGraphics.draw(text);

		ypos += text.getFont().getLineSpacing(mTextSize);
	}

}

void Messenger::add(const wstring& text, sf::Color color)
{
	shared_ptr<Entry> entry = boost::make_shared<Entry>(text);
	entry->myText.setColor(color);
	entry->myText.setCharacterSize(mTextSize);
	mEntries.push_back(entry);
}


Messenger::int_type Messenger::overflow(Messenger::int_type c )
{
	wchar_t wc = c;

	if(c == L'\n')
	{
		add(mNextLine);
		mNextLine.clear();

	}else{
		mNextLine += c;
	}

	// jedes Zeichen an BEIDE Streambufs sb1 & sb2 weitergeben
	if( mOldSB && !traits_type::eq_int_type( mOldSB->sputc( c ), traits_type::eof() ) )
		return traits_type::not_eof( c );
	return traits_type::eof();  // einer der Streambufs tut nicht
}

}