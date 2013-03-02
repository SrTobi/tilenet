#pragma once
#ifndef _NET_BUFFER_BASE_H
#define _NET_BUFFER_BASE_H



#include "settings.h"

namespace net {

class BufferBase
{
	// A bool-like type that cannot be converted to integer or pointer types
	typedef bool (BufferBase::*BoolType)(std::size_t);

public :
	typedef sf::Uint8					IDType;
	typedef std::map<string, wstring>	Map;

	BufferBase(IDType id);
	virtual ~BufferBase();


	const void*		getData() const;
	std::size_t		getDataSize() const;
	void*			getMutableData();

	const IDType	id() const;


	operator BoolType() const;


protected:
	const void* getReadAddress() const;

	////////////////////////////////////////////////////////////
	/// Disallow comparisons between packets
	///
	////////////////////////////////////////////////////////////
	bool operator ==(const BufferBase& right) const;
	bool operator !=(const BufferBase& right) const;

	////////////////////////////////////////////////////////////
	/// \brief Check if the packet can extract a given number of bytes
	///
	/// This function updates accordingly the state of the packet.
	///
	/// \param size Size to check
	///
	/// \return True if \a size bytes can be read from the packet
	///
	////////////////////////////////////////////////////////////
	bool checkSize(std::size_t size);

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	std::vector<sf::Uint8> m_data; ///< Data stored in the packet
	std::size_t m_readPos; ///< Current reading position in the packet
	bool m_isValid; ///< Reading state of the packet
};


}

template<typename T>
inline std::basic_ostream<T>& operator << (std::basic_ostream<T>& os, const net::BufferBase::Map& map)
{
	for(auto it = map.begin();
		it != map.end();
		++it)
	{
		os << it->first.c_str() << L"=\"" << it->second << L"\"";
	}

	return os;
}


#endif