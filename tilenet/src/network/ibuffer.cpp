#include "includes.h"
#include "network/ibuffer.h"


using namespace sf;

namespace net {

	
IBuffer::IBuffer()
	: BufferBase(0)
{
}

void IBuffer::prepare(uint size)
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
	Uint8 value;
	if (*this >> value)
		data = (value != 0);

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Int8& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const Int8*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Uint8& data)
{
	if (checkSize(sizeof(data)))
	{
		data = *reinterpret_cast<const Uint8*>(&m_data[m_readPos]);
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Int16& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohs(*reinterpret_cast<const Int16*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Uint16& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohs(*reinterpret_cast<const Uint16*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Int32& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohl(*reinterpret_cast<const Int32*>(&m_data[m_readPos]));
		m_readPos += sizeof(data);
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(Uint32& data)
{
	if (checkSize(sizeof(data)))
	{
		data = ntohl(*reinterpret_cast<const Uint32*>(&m_data[m_readPos]));
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
	Uint32 length = 0;
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
	Uint32 length = 0;
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
	Uint32 out;

	*this >> out;

	wch = static_cast<wchar>(out);

	return *this;
}

////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(wchar_t* data)
{
	// First extract string length
	Uint32 length = 0;
	*this >> length;

	if ((length > 0) && checkSize(length * sizeof(Uint32)))
	{
		// Then extract characters
		for (Uint32 i = 0; i < length; ++i)
		{
			Uint32 character = 0;
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
	Uint32 length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length * sizeof(Uint32)))
	{
		// Then extract characters
		for (Uint32 i = 0; i < length; ++i)
		{
			Uint32 character = 0;
			*this >> character;
			data += static_cast<wchar_t>(character);
		}
	}

	return *this;
}


////////////////////////////////////////////////////////////
IBuffer& IBuffer::operator >>(String& data)
{
	// First extract the string length
	Uint32 length = 0;
	*this >> length;

	data.clear();
	if ((length > 0) && checkSize(length * sizeof(Uint32)))
	{
		// Then extract characters
		for (Uint32 i = 0; i < length; ++i)
		{
			Uint32 character = 0;
			*this >> character;
			data += character;
		}
	}

	return *this;
}


IBuffer& IBuffer::operator >>(Map& data)
{	
	// First extract the string length
	Uint32 length = 0;
	*this >> length;

	std::pair<string, wstring> insert_pair;

	for(Uint32 i = 0;  i < length; ++i)
	{
		*this >> insert_pair.first;

		if(!insert_pair.first.size())
			break;

		*this >> insert_pair.second;

		data.insert(insert_pair);
	}


	return *this;
}

}