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

	m_readIndex = sourceBuffer.m_readIndex;
	m_writeIndex = sourceBuffer.m_writeIndex;

	sourceBuffer.clearIndexes();
}

void Buffer::copyBufferContent(const Buffer & sourceBuffer)
{
	m_data = sourceBuffer.m_data;
	m_readIndex = sourceBuffer.m_readIndex;
	m_writeIndex = sourceBuffer.m_writeIndex;
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

void Buffer::clear()
{
	m_data.fillWithZeroes();
	clearIndexes();
}

void Buffer::clearIndexes()
{
	m_readIndex = 0;
	m_writeIndex = 0;
}

void Buffer::write(const Byte_8 & byte) 
{
	m_data[m_writeIndex] = byte;
	m_writeIndex++;
}

void Buffer::write(const ByteArray & byteArray) //TODO
{
}

Byte_8 Buffer::read() const
{
	m_readIndex--;
	return m_data[m_readIndex];
}

ByteArray Buffer::read(const int & size) const //TODO
{
	return ByteArray();
}

const srl::Byte_8 & Buffer::data() const
{
	return m_data[0];
}

int Buffer::getReadIndex() const
{
	return m_readIndex;
}

int Buffer::getWriteIndex() const
{
	return m_writeIndex;
}

void Buffer::setReadIndex(const int & index) const
{
	m_readIndex = index;
}

void Buffer::setWriteIndex(const int & index)
{
	m_writeIndex = index;
}

void Buffer::fitInternalBuffer()
{
	m_data.resize(BUFFER_SIZE);
}

} //end of namespace