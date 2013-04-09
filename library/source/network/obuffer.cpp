#include "includes.hpp"
#include "network/obuffer.hpp"


namespace net {

OBuffer::OBuffer(id_type id)
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


void OBuffer::id(id_type id)
{
	m_data[0] = id;
}


std::vector<uint8>&& OBuffer::buffer()
{
	return std::move(m_data);
}



////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(bool data)
{
	*this << static_cast<uint8>(data);
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(int8 data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(uint8 data)
{
	append(&data, sizeof(data));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(int16 data)
{
	int16 toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(uint16 data)
{
	uint16 toWrite = htons(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(int32 data)
{
	int32 toWrite = htonl(data);
	append(&toWrite, sizeof(toWrite));
	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(uint32 data)
{
	uint32 toWrite = htonl(data);
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
	uint32 length = 0;
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
	uint32 length = static_cast<uint32>(data.size());
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
	*this << static_cast<uint32>(data);

	return *this;
}

////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const wchar_t* data)
{
	// First insert string length
	uint32 length = 0;
	for (const wchar_t* c = data; *c != L'\0'; ++c)
		++length;
	*this << length;

	// Then insert characters
	for (const wchar_t* c = data; *c != L'\0'; ++c)
		*this << static_cast<uint32>(*c);

	return *this;
}


////////////////////////////////////////////////////////////
OBuffer& OBuffer::operator <<(const std::wstring& data)
{
	// First insert string length
	uint32 length = static_cast<uint32>(data.size());
	*this << length;

	// Then insert characters
	if (length > 0)
	{
		for (std::wstring::const_iterator c = data.begin(); c != data.end(); ++c)
			*this << static_cast<uint32>(*c);
	}

	return *this;
}


}