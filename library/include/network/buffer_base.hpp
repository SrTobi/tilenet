#pragma once
#ifndef _BUFFER_BASE_HPP
#define _BUFFER_BASE_HPP

#include <map>

#include "settings.hpp"

namespace net {

class BufferBase
{
	// A bool-like type that cannot be converted to integer or pointer types
	typedef bool (BufferBase::*BoolType)(std::size_t);

public :
	typedef MsgId id_type;

	BufferBase(id_type id);
	virtual ~BufferBase();


	const void*		getData() const;
	std::size_t		getDataSize() const;
	void*			getMutableData();

	const id_type	id() const;


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
	std::vector<uint8> m_data; ///< Data stored in the packet
	std::size_t m_readPos; ///< Current reading position in the packet
	bool m_isValid; ///< Reading state of the packet
};


}

#endif