#pragma once
#ifndef _PROTOCOL_BUFFER_HPP
#define _PROTOCOL_BUFFER_HPP

#include <vector>
#include <codecvt>
#include <boost/concept_check.hpp>
#include "settings.hpp"


namespace net {

struct access
{
};

namespace helper {

template<typename T>
struct NumberSerializer
{
	static_assert(std::is_integral<T>::value, "T is not a numeric type!");

	void operator () (ProtocolOBuffer<T>& buffer, T n)
	{
		static const char* hexNumberSet = "0123456789abcdef";

		const unsigned int bits_per_part = 4;
		const unsigned int parts_per_byte = 2;
		const unsigned int parts_per_uint = sizeof(n) * parts_per_byte;
		const unsigned int part_mask = 0xF;

		unsigned int mask_shift = parts_per_uint;

		std::make_unsigned<T> number;

		if(std::is_signed<T>::value && n < 0)
		{
			buffer.put(L'-');
			number = -n;
		}else{
			number = n;
		}


		do
		{
			--mask_shift;
			unsigned int part = (number >> (mask_shift * bits_per_part)) & part_mask;
			assert(part <= part_mask);
			buffer.put(hexNumberSet[part]);

		} while(mask_shift);
	}
};

}


template<typename BufferType = std::vector<char> >
class ProtocolOBuffer
{
public:
	typedef BufferType buffer_type;


	static_cast(std::is_same<typename buffer_type::value_type, char>::value, "Container must contain char");
	BOOST_CONCEPT_ASSERT((boost::BackInsertionSequence<buffer_type>));

	ProtocolOBuffer(buffer_type& buffer) : mBuffer(buffer) {}
	~ProtocolOBuffer() {}

	void put(char c)
	{
		tnAssert(c > 0 && c <= 127);
		mBuffer.push_back(c);
	}

	void put(wchar_t c)
	{
		tnAssert(c != 0);

		// Convert to utf8
		std::wstring_convert<std::codecvt_utf8<wchar_t> > converter;
		std::string utf8string = converter.to_bytes(c);
		std::copy(utf8string.begin(), utf8string.end(), std::back_inserter(mBuffer));
	}

private:
	buffer_type& mBuffer;
};

#define SERIALIZER_FOR_NUMERIC_TYPE(_type) template<typename T> ProtocolOBuffer<T>& operator << (ProtocolOBuffer<T>& buffer, _type n) { helper::NumberSerializer<_type> s; s(n); return *this; }


template<typename T>
ProtocolOBuffer<T>& operator << (ProtocolOBuffer<T>& buffer, char b)
{
	buffer.put(b? '1' : '0');

	return *this;
}


SERIALIZER_FOR_NUMERIC_TYPE(unsigned char);
SERIALIZER_FOR_NUMERIC_TYPE(short);
SERIALIZER_FOR_NUMERIC_TYPE(unsigned short);
SERIALIZER_FOR_NUMERIC_TYPE(int);
SERIALIZER_FOR_NUMERIC_TYPE(unsigned int);
SERIALIZER_FOR_NUMERIC_TYPE(long);
SERIALIZER_FOR_NUMERIC_TYPE(unsigned long);
SERIALIZER_FOR_NUMERIC_TYPE(long long);
SERIALIZER_FOR_NUMERIC_TYPE(unsigned long long);

template<typename T>
ProtocolOBuffer<T>& operator << (ProtocolOBuffer<T>& buffer, bool b)
{
	buffer.put(b? '1' : '0');

	return *this;
}


}









#endif