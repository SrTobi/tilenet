#include "includes.hpp"
#include "network/ibuffer.hpp"



namespace net {

	
IBuffer::IBuffer()
	: BufferBase(0)
{
}

void IBuffer::prepare(unsigned int size)
{
	m_data.resize(size);
	m_readPos = 1;
	m_isValid = true;
}


bool IBuffer::endOfBuffer() const
{
	return m_readPos >= m_data.size();
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(bool& data)
{
	uint8 value;
	if (*this >> value)
		data = (value != 0);

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(int8& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const int8*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(uint8& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const uint8*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(int16& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohs(*reinterpret_cast<const int16*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(uint16& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohs(*reinterpret_cast<const uint16*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(int32& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohl(*reinterpret_cast<const int32*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(uint32& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohl(*reinterpret_cast<const uint32*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(float& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const float*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(double& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const double*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(char* data)
{
	// First extract string length
	uint32 length = 0;
	*this >> length;

	if ((length > 0) && checkSize(length))
	{
		// Then extract characters
		std::memcpy(data, &m_data[m_readPos], length);
		data[length] = '\0';

		// Update reading position
		m_readPos += length;
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(std::string& data)
{
	// First extract string length
	uint32 length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length))
	{
		// Then extract characters
		data.assign(reinterpret_cast<char*>(&m_data[m_readPos]), length);

		// Update reading position
		m_readPos += length;
	}

	return *this;
}

////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(wchar_t& wch)
{
	// First extract string length
	uint32 out;

	*this >> out;

	wch = static_cast<wchar_t>(out);

	return *this;
}

////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(wchar_t* data)
{
	// First extract string length
	uint32 length = 0;
	*this >> length;

	if ((length > 0) && checkSize(length * sizeof(uint32)))
	{
		// Then extract characters
		for (uint32 i = 0; i < length; ++i)
		{
			uint32 character = 0;
			*this >> character;
			data[i] = static_cast<wchar_t>(character);
		}
		data[length] = L'\0';
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(std::wstring& data)
{
	// First extract string length
	uint32 length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length * sizeof(uint32)))
	{
		// Then extract characters
		for (uint32 i = 0; i < length; ++i)
		{
			uint32 character = 0;
			*this >> character;
			data += static_cast<wchar_t>(character);
		}
	}

	return *this;
}

}