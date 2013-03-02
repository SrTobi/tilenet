#pragma once
#ifndef _NET_OBUFFER_H
#define _NET_OBUFFER_H


#include "settings.h"
#include "buffer_base.h"


namespace net {

class OBuffer
	: public BufferBase
{
public :
	OBuffer(IDType id = 0 /* 0 is error */);

	void					append(const void* data, std::size_t sizeInBytes);
	void					id(IDType id);
	asio::const_buffers_1	buffer() const;

public:
	////////////////////////////////////////////////////////////
	/// Overloads of operator << to write data into the packet
	///
	////////////////////////////////////////////////////////////
	OBuffer& operator <<(bool data);
	OBuffer& operator <<(sf::Int8 data);
	OBuffer& operator <<(sf::Uint8 data);
	OBuffer& operator <<(sf::Int16 data);
	OBuffer& operator <<(sf::Uint16 data);
	OBuffer& operator <<(sf::Int32 data);
	OBuffer& operator <<(sf::Uint32 data);
	OBuffer& operator <<(float data);
	OBuffer& operator <<(double data);
	OBuffer& operator <<(const char* data);
	OBuffer& operator <<(const std::string& data);
	OBuffer& operator <<(wchar_t data);
	OBuffer& operator <<(const wchar_t* data);
	OBuffer& operator <<(const std::wstring& data);
	OBuffer& operator <<(const sf::String& data);
	OBuffer& operator <<(const Map& data);

	template<typename T>
	OBuffer& operator <<(tn::basic_vector2<T>& v)
	{
		*this << v.x << v.y;

		return *this;
	}
};





}


#endif