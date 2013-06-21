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
	assert(key != sf::Keyboard::Unknown);
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
	addCode(L"a", sf::Keyboard::A);
	addCode(L"b", sf::Keyboard::B);
	addCode(L"c", sf::Keyboard::C);
	addCode(L"d", sf::Keyboard::D);
	addCode(L"e", sf::Keyboard::E);
	addCode(L"f", sf::Keyboard::F);
	addCode(L"g", sf::Keyboard::G);
	addCode(L"h", sf::Keyboard::H);
	addCode(L"i", sf::Keyboard::I);
	addCode(L"j", sf::Keyboard::J);
	addCode(L"k", sf::Keyboard::K);
	addCode(L"l", sf::Keyboard::L);
	addCode(L"n", sf::Keyboard::N);
	addCode(L"m", sf::Keyboard::M);
	addCode(L"o", sf::Keyboard::O);
	addCode(L"p", sf::Keyboard::P);
	addCode(L"q", sf::Keyboard::Q);
	addCode(L"r", sf::Keyboard::R);
	addCode(L"s", sf::Keyboard::S);
	addCode(L"t", sf::Keyboard::T);
	addCode(L"u", sf::Keyboard::U);
	addCode(L"v", sf::Keyboard::V);
	addCode(L"w", sf::Keyboard::W);
	addCode(L"x", sf::Keyboard::X);
	addCode(L"y", sf::Keyboard::Y);
	addCode(L"z", sf::Keyboard::Z);
	addCode(L"0", sf::Keyboard::Num0);
	addCode(L"1", sf::Keyboard::Num1);
	addCode(L"2", sf::Keyboard::Num2);
	addCode(L"3", sf::Keyboard::Num3);
	addCode(L"4", sf::Keyboard::Num4);
	addCode(L"5", sf::Keyboard::Num5);
	addCode(L"6", sf::Keyboard::Num6);
	addCode(L"7", sf::Keyboard::Num7);
	addCode(L"8", sf::Keyboard::Num8);
	addCode(L"9", sf::Keyboard::Num9);
	addCode(L"escape|esc", sf::Keyboard::Escape);
	addCode(L"control|l-control", sf::Keyboard::LControl);
	addCode(L"shift|l-shift", sf::Keyboard::LShift);
	addCode(L"alt|l-alt", sf::Keyboard::LAlt);
	addCode(L"system|l-system", sf::Keyboard::LSystem);
	addCode(L"r-control", sf::Keyboard::RControl);
	addCode(L"r-shift", sf::Keyboard::RShift);
	addCode(L"r-alt", sf::Keyboard::RAlt);
	addCode(L"r-system", sf::Keyboard::RSystem);
	addCode(L"menu|menue", sf::Keyboard::Menu);
	addCode(L"l-bracket", sf::Keyboard::LBracket);
	addCode(L"r-bracket", sf::Keyboard::RBracket);
	addCode(L"semicolon|;", sf::Keyboard::SemiColon);
	addCode(L"comma|,", sf::Keyboard::Comma);
	addCode(L"period|.|point|fullstop|dot", sf::Keyboard::Period);
	addCode(L"quote|'", sf::Keyboard::Quote);
	addCode(L"slash|/", sf::Keyboard::Slash);
	addCode(L"backslash|\\", sf::Keyboard::BackSlash);
	addCode(L"tilde|~", sf::Keyboard::Tilde);
	addCode(L"equal|=", sf::Keyboard::Equal);
	addCode(L"dash", sf::Keyboard::Dash);
	addCode(L"space", sf::Keyboard::Space);
	addCode(L"return|enter", sf::Keyboard::Return);
	addCode(L"backspace", sf::Keyboard::BackSpace);
	addCode(L"tab", sf::Keyboard::Tab);
	addCode(L"pageup", sf::Keyboard::PageUp);
	addCode(L"pagedown", sf::Keyboard::PageDown);
	addCode(L"end", sf::Keyboard::End);
	addCode(L"home|pos1", sf::Keyboard::Home);
	addCode(L"insert", sf::Keyboard::Insert);
	addCode(L"delete", sf::Keyboard::Delete);
	addCode(L"add|plus", sf::Keyboard::Add);
	addCode(L"subtract|minus", sf::Keyboard::Subtract);
	addCode(L"multiply", sf::Keyboard::Multiply);
	addCode(L"divide", sf::Keyboard::Divide);
	addCode(L"left", sf::Keyboard::Left);
	addCode(L"right", sf::Keyboard::Right);
	addCode(L"up", sf::Keyboard::Up);
	addCode(L"down", sf::Keyboard::Down);
	addCode(L"numpad0", sf::Keyboard::Numpad0);
	addCode(L"numpad1", sf::Keyboard::Numpad1);
	addCode(L"numpad2", sf::Keyboard::Numpad2);
	addCode(L"numpad3", sf::Keyboard::Numpad3);
	addCode(L"numpad4", sf::Keyboard::Numpad4);
	addCode(L"numpad5", sf::Keyboard::Numpad5);
	addCode(L"numpad6", sf::Keyboard::Numpad6);
	addCode(L"numpad7", sf::Keyboard::Numpad7);
	addCode(L"numpad8", sf::Keyboard::Numpad8);
	addCode(L"numpad9", sf::Keyboard::Numpad9);
	addCode(L"f1", sf::Keyboard::F1);
	addCode(L"f2", sf::Keyboard::F2);
	addCode(L"f3", sf::Keyboard::F3);
	addCode(L"f4", sf::Keyboard::F4);
	addCode(L"f5", sf::Keyboard::F5);
	addCode(L"f6", sf::Keyboard::F6);
	addCode(L"f7", sf::Keyboard::F7);
	addCode(L"f8", sf::Keyboard::F8);
	addCode(L"f9", sf::Keyboard::F9);
	addCode(L"f10", sf::Keyboard::F10);
	addCode(L"f11", sf::Keyboard::F11);
	addCode(L"f12", sf::Keyboard::F12);
	addCode(L"f13", sf::Keyboard::F13);
	addCode(L"f14", sf::Keyboard::F14);
	addCode(L"f15", sf::Keyboard::F15);
	addCode(L"pause", sf::Keyboard::Pause);
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