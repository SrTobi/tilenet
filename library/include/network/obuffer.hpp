#pragma once
#ifndef _OBUFFER_HPP
#define _OBUFFER_HPP


#include "settings.hpp"
#include "buffer_base.hpp"


namespace net {

class OBuffer
	: public BufferBase
{
public :
	OBuffer(id_type id = 0 /* 0 is error */);

	void					append(const void* data, std::size_t sizeInBytes);
	void					id(id_type id);
	std::vector<uint8>&&	buffer();

public:
	////////////////////////////////////////////////////////////
	/// Overloads of operator << to write data into the packet
	///
	////////////////////////////////////////////////////////////
	OBuffer& operator <<(bool data);
	OBuffer& operator <<(int8 data);
	OBuffer& operator <<(uint8 data);
	OBuffer& operator <<(int16 data);
	OBuffer& operator <<(uint16 data);
	OBuffer& operator <<(int32 data);
	OBuffer& operator <<(uint32 data);
	OBuffer& operator <<(float data);
	OBuffer& operator <<(double data);
	OBuffer& operator <<(const char* data);
	OBuffer& operator <<(const std::string& data);
	OBuffer& operator <<(wchar_t data);
	OBuffer& operator <<(const wchar_t* data);
	OBuffer& operator <<(const std::wstring& data);

	template<typename T>
	OBuffer& operator <<(const basic_vector2<T>& v)
	{
		*this << v.x << v.y;

		return *this;
	}
};





}


#endif