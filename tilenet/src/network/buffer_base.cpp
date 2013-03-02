#include "includes.h"
#include "network/buffer_base.h"



namespace net {

////////////////////////////////////////////////////////////
BufferBase::BufferBase(IDType id)
	:	m_readPos(1),
		m_isValid(true)
{
	// Keine id!
	m_data.push_back(id);
}

BufferBase::~BufferBase()
{
}

////////////////////////////////////////////////////////////
const void* BufferBase::getData() const
{
    return &m_data[0];
}


////////////////////////////////////////////////////////////
std::size_t BufferBase::getDataSize() const
{
    return m_data.size();
}


const void* BufferBase::getReadAddress() const
{
	return &(m_data[m_readPos]);
}

void* BufferBase::getMutableData()
{
    return !m_data.empty() ? &m_data[0] : nullptr;
}


const BufferBase::IDType BufferBase::id() const
{
	return m_data[0];
}

////////////////////////////////////////////////////////////
BufferBase::operator BoolType() const
{
    return m_isValid ? & BufferBase::checkSize : nullptr;
}


////////////////////////////////////////////////////////////
bool BufferBase::checkSize(std::size_t size)
{
    m_isValid = m_isValid && (m_readPos + size <= m_data.size());

    return m_isValid;
}













}