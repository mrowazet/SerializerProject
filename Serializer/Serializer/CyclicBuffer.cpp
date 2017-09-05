#include "CyclicBuffer.h"

namespace srl
{

CyclicBuffer::CyclicBuffer(const unsigned int & bufferSize)
	:BUFFER_SIZE(bufferSize)
{
	fitInternalBuffer();
}

CyclicBuffer::~CyclicBuffer()
{
}

CyclicBuffer::CyclicBuffer(const CyclicBuffer & sourceBuffer)
{
	copyBufferContent(sourceBuffer);
}

CyclicBuffer::CyclicBuffer(CyclicBuffer && sourceBuffer)
{
	moveBufferContent(std::move(sourceBuffer));
}

CyclicBuffer & CyclicBuffer::operator=(const CyclicBuffer & sourceBuffer)
{	
	copyBufferContent(sourceBuffer);
	return *this;
}

CyclicBuffer & CyclicBuffer::operator=(CyclicBuffer && sourceBuffer)
{	
	moveBufferContent(std::move(sourceBuffer));
	return *this;
}

void CyclicBuffer::moveBufferContent(CyclicBuffer && sourceBuffer)
{
	m_data = std::move(sourceBuffer.m_data);

	BUFFER_SIZE = sourceBuffer.BUFFER_SIZE;
	sourceBuffer.m_data.resize(BUFFER_SIZE);

	m_index = sourceBuffer.m_index;
	sourceBuffer.m_index = 0;
}

void CyclicBuffer::copyBufferContent(const CyclicBuffer & sourceBuffer)
{
	m_data = sourceBuffer.m_data;
	m_index = sourceBuffer.m_index;
	BUFFER_SIZE = sourceBuffer.BUFFER_SIZE;
}

unsigned int CyclicBuffer::size() const
{
	return m_data.size();
}

bool CyclicBuffer::isEmpty() const
{
	return m_data.isZeroed();
}

CyclicBuffer::operator bool() const
{
	return isEmpty();
}

void CyclicBuffer::clear() //TODO add perf test for that and check another impls
{
	m_index = 0;

	for (int i = 0; i < m_data.size(); i++)
		m_data[i].clear();
}

void CyclicBuffer::write(const Byte_8 & byte) //TODO what if current index is eq max size, add TC
{
	m_data[m_index] = byte;
	m_index++;
}

void CyclicBuffer::write(const ByteArray & byteArray) //TODO
{
}

Byte_8 CyclicBuffer::read() const //TODO what if current index is 0, add TCs
{
	m_index--;
	return m_data[m_index];
}

ByteArray CyclicBuffer::read(const int & size) const //TODO
{
	return ByteArray();
}

const srl::Byte_8 & CyclicBuffer::data() const
{
	return m_data[0];
}

void CyclicBuffer::fitInternalBuffer()
{
	m_data.resize(BUFFER_SIZE);
}

} //end of namespace