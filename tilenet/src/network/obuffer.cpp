#include "includes.h"
#include "network/obuffer.h"

using namespace sf;

namespace net {

OBuffer::OBuffer(IDType id)
	: BufferBase(id)
{
}


void OBuffer::append(const void* data, std::size_t sizeInBytes)
{
	if (data && (sizeInBytes > 0))
	{
		std::size_t start = m_data.size();
		m_data.resize(start + sizeInBytes);
		std::memcpy(&m_data[start], data, sizeInBytes);
	}
}


void OBuffer::id(IDType id)
{
	m_data[0] = id;
}


asio::const_buffers_1 OBuffer::buffer() const
{
	return asio::buffer(getData(), getDataSize());
}



////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(bool data)
{
	*this << static_cast<Uint8>(data);
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Int8 data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Uint8 data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Int16 data)
{
	Int16 toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Uint16 data)
{
	Uint16 toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Int32 data)
{
	Int32 toWrite = htonl(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(Uint32 data)
{
	Uint32 toWrite = htonl(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(float data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(double data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const char* data)
{
	// First insert string length
	Uint32 length = 0;
	for (const char* c = data; *c != '\0'; ++c)
		++length;
	*this << length;

	// Then insert characters
	append(data, length * sizeof(char));

	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const std::string& data)
{
	// First insert string length
	Uint32 length = static_cast<Uint32>(data.size());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		append(data.c_str(), length * sizeof(std::string::value_type));
	}

	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(wchar_t data)
{
	*this << static_cast<Uint32>(data);

	return *this;
}

////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const wchar_t* data)
{
	// First insert string length
	Uint32 length = 0;
	for (const wchar_t* c = data; *c != L'\0'; ++c)
		++length;
	*this << length;

	// Then insert characters
	for (const wchar_t* c = data; *c != L'\0'; ++c)
		*this << static_cast<Uint32>(*c);

	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const std::wstring& data)
{
	// First insert string length
	Uint32 length = static_cast<Uint32>(data.size());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
			*this << static_cast<Uint32>(*c);
	}

	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const String& data)
{
	// First insert the string length
	Uint32 length = static_cast<Uint32>(data.getSize());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		for (String::ConstIterator c = data.begin(); c != data.end(); ++c)
			*this << *c;
	}

	return *this;
}

OBuffer& OBuffer::operator <<(const Map& data)
{
	auto empty_elem = data.find("");

	// First insert the string length
	Uint32 length = static_cast<Uint32>(data.size() - (empty_elem == data.end() ? 0 : 1));
	*this << length;

	for(auto it = data.begin();
		it != data.end();
		++it)
	{
		if(it != empty_elem)
			*this << it->first << it->second;
	}

	return *this;
}

}