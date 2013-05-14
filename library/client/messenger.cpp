#include "includes.hpp"
#include "messenger.hpp"



namespace client {


	
Messenger::Messenger(unsigned int maxEntries, unsigned int entryLiveTime, unsigned int textSize)
	:	mEntryLiveTime(entryLiveTime),
		mMaxEntries(maxEntries),
		mTextSize(textSize)
{
	if(!mFont.loadFromFile("cour.ttf"))
	{
		Log(L"client").error() << "Failed to load font for messenger!";
	}
}

Messenger::~Messenger()
{
}

void Messenger::render(sf::RenderTarget& g)
{
	// if too many entries exists, pop some
	while(mEntries.size() > mMaxEntries)
		mEntries.pop_front();

	// erase all dead entries
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

		g.draw(text);

		ypos += mFont.getLineSpacing(mTextSize);
	}

}

void Messenger::add(const string& text, sf::Color color)
{
	shared_ptr<Entry> entry(new Entry(text, mFont));
	entry->myText.setColor(color);
	entry->myText.setCharacterSize(mTextSize);
	mEntries.push_back(entry);
}

}