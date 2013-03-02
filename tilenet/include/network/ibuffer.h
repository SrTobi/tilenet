#pragma once
#ifndef _NET_IBUFFER_H
#define _NET_IBUFFER_H



#include "settings.h"
#include "buffer_base.h"


namespace net {

class IBuffer
	: public BufferBase
{
public:
	template<typename T, typename C>
	struct AutoChecker: public T { C checker; };
public:
	IBuffer();
	
	// Prepare to read!
	void prepare(uint size);
	bool endOfBuffer() const;

public:

	////////////////////////////////////////////////////////////
	/// Overloads of operator >> to read data from the packet
	///
	////////////////////////////////////////////////////////////
	IBuffer& operator >>(bool& data);
	IBuffer& operator >>(sf::Int8& data);
	IBuffer& operator >>(sf::Uint8& data);
	IBuffer& operator >>(sf::Int16& data);
	IBuffer& operator >>(sf::Uint16& data);
	IBuffer& operator >>(sf::Int32& data);
	IBuffer& operator >>(sf::Uint32& data);
	IBuffer& operator >>(float& data);
	IBuffer& operator >>(double& data);
	IBuffer& operator >>(char* data);
	IBuffer& operator >>(std::string& data);
	IBuffer& operator >>(wchar_t& wch);
	IBuffer& operator >>(wchar_t* data);
	IBuffer& operator >>(std::wstring& data);
	IBuffer& operator >>(sf::String& data);
	IBuffer& operator >>(Map& data);

	template<typename T>
	IBuffer& operator >>(tn::basic_vector2<T>& v)
	{
		T i;
		*this >> i;
		v.x = i;
		*this >> i;
		v.y = i;

		return *this;
	}

	template<typename T, typename C>
	IBuffer& operator >>(AutoChecker<T, C>& checker)
	{
		*this >> static_cast<T&>(checker);
		if(!checker.checker(checker))
		{
			throw tn::TnException(L"Failed to extract from package!");
		}
	}
};





}
#endif