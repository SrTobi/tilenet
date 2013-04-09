#pragma once
#ifndef _IBUFFER_HPP
#define _IBUFFER_HPP



#include "settings.hpp"
#include "buffer_base.hpp"


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
	void prepare(unsigned int size);
	bool endOfBuffer() const;

public:

	////////////////////////////////////////////////////////////
	/// Overloads of operator >> to read data from the packet
	///
	////////////////////////////////////////////////////////////
	IBuffer& operator >>(bool& data);
	IBuffer& operator >>(int8& data);
	IBuffer& operator >>(uint8& data);
	IBuffer& operator >>(int16& data);
	IBuffer& operator >>(uint16& data);
	IBuffer& operator >>(int32& data);
	IBuffer& operator >>(uint32& data);
	IBuffer& operator >>(float& data);
	IBuffer& operator >>(double& data);
	IBuffer& operator >>(char* data);
	IBuffer& operator >>(std::string& data);
	IBuffer& operator >>(wchar_t& wch);
	IBuffer& operator >>(wchar_t* data);
	IBuffer& operator >>(std::wstring& data);

	template<typename T>
	IBuffer& operator >>(const basic_vector2<T>& v)
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