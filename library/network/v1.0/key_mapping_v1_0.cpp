#include "includes.hpp"
#include "key_mapping_v1_0.hpp"


namespace proto {
namespace v1_0 {

KeyMapper::KeyMapper()
	: mNextCode(0)
{
	init();
}

KeyMapper::~KeyMapper()
{
}

bool KeyMapper::toCode(const string& name, TNKEYCODE* code) const
{
	auto it = mNameToCodeMapping.find(name);
	if(it == mNameToCodeMapping.end())
	{
		return false;
	}

	*code = it->second;
	return true;
}

TNKEYCODE KeyMapper::toCode( sf::Keyboard::Key key ) const
{
	auto it = mSfmlToCodeMapping.find(key);

	tnAssert(it != mSfmlToCodeMapping.end());

	return it->second;
}

const string& KeyMapper::toName( TNKEYCODE code ) const
{
	if(code >= mCodeToMapping.size() || !mCodeToMapping[code])
		return mEmptyString;
	return *mCodeToMapping[code];
}

const KeyMapper& KeyMapper::Inst()
{
	static KeyMapper Singleton;
	return Singleton;
}

void KeyMapper::init()
{
	addCode(L"escape|esc", sf::Keyboard::Escape);
}

void KeyMapper::addCode( const string& names, sf::Keyboard::Key sfmlKey )
{
	const unsigned int code = mNextCode++;
	{
		std::wistringstream iss(names);
		string name;
		bool first = true;

		while(std::getline(iss, name, L'|'))
		{
			auto res = mNameToCodeMapping.emplace(name, code);
			assert(res.second);

			if(first)
			{
				first = false;
				tnAssert(mCodeToMapping.size() == code);
				mCodeToMapping.push_back(&(res.first->first));
			}
		}


	}

	{
		auto res = mSfmlToCodeMapping.emplace(sfmlKey, code);
		assert(res.second);
	}
}





}}