#pragma once
#ifndef _KEY_MAPPING_V1_0_HPP
#define _KEY_MAPPING_V1_0_HPP

#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>
#include <vector>
#include "settings.hpp"


namespace proto {
namespace v1_0 {


class KeyMapper
{
public:
	KeyMapper();
	~KeyMapper();

	bool toCode(const string& name, TNKEYCODE* code) const;
	TNKEYCODE toCode(sf::Keyboard::Key key) const;

	const string& toName(TNKEYCODE code) const;

	static const KeyMapper& Inst();

private:
	void init();
	void addCode(const string& names, sf::Keyboard::Key sfmlKey);
private:
	const string mEmptyString;
	std::vector<const string*> mCodeToMapping;
	std::unordered_map<string, TNKEYCODE> mNameToCodeMapping;
	std::unordered_map<sf::Keyboard::Key, TNKEYCODE> mSfmlToCodeMapping;
	unsigned int mNextCode;
};



}}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, TNEVENT::keyevent_type& data, const unsigned int version)
{
	ar & data.key & data.modifier & data.ch;
}



} // namespace serialization
} // namespace boost








#endif