#include "Buffer.h"

namespace srl
{

Buffer::Buffer(const unsigned int & bufferSize)
	:BUFFER_SIZE(bufferSize)
{
	fitInternalBuffer();
}

Buffer::~Buffer()
{
}

Buffer::Buffer(const Buffer & sourceBuffer)
{
	copyBufferContent(sourceBuffer);
}

Buffer::Buffer(Buffer && sourceBuffer)
{
	moveBufferContent(std::move(sourceBuffer));
}

Buffer & Buffer::operator=(const Buffer & sourceBuffer)
{	
	copyBufferContent(sourceBuffer);
	return *this;
}

Buffer & Buffer::operator=(Buffer && sourceBuffer)
{	
	moveBufferContent(std::move(sourceBuffer));
	return *this;
}

void Buffer::moveBufferContent(Buffer && sourceBuffer)
{
	m_data = std::move(sourceBuffer.m_data);

	BUFFER_SIZE = sourceBuffer.BUFFER_SIZE;
	sourceBuffer.m_data.resize(BUFFER_SIZE);

	m_index = sourceBuffer.m_index;
	sourceBuffer.m_index = 0;
}

void Buffer::copyBufferContent(const Buffer & sourceBuffer)
{
	m_data = sourceBuffer.m_data;
	m_index = sourceBuffer.m_index;
	BUFFER_SIZE = sourceBuffer.BUFFER_SIZE;
}

unsigned int Buffer::size() const
{
	return m_data.size();
}

bool Buffer::isEmpty() const
{
	return m_data.isZeroed();
}

Buffer::operator bool() const
{
	return isEmpty();
}

void Buffer::clear() //TODO add perf test for that and check another impls
{
	m_index = 0;

	for (int i = 0; i < m_data.size(); i++)
		m_data[i].clear();
}

void Buffer::write(const Byte_8 & byte) //TODO what if current index is eq max size, add TC
{
	m_data[m_index] = byte;
	m_index++;
}

void Buffer::write(const ByteArray & byteArray) //TODO
{
}

Byte_8 Buffer::read() const //TODO what if current index is 0, add TCs
{
	m_index--;
	return m_data[m_index];
}

ByteArray Buffer::read(const int & size) const //TODO
{
	return ByteArray();
}

const srl::Byte_8 & Buffer::data() const
{
	return m_data[0];
}

void Buffer::fitInternalBuffer()
{
	m_data.resize(BUFFER_SIZE);
}

} //end of namespace