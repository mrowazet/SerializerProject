#include "Buffer.h"

namespace srl
{

Buffer::Buffer(const unsigned int & bufferSize)
	:m_bufferSize(bufferSize)
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
	if(this != &sourceBuffer)
		copyBufferContent(sourceBuffer);

	return *this;
}

Buffer & Buffer::operator=(Buffer && sourceBuffer)
{	
	if(this != &sourceBuffer)
		moveBufferContent(std::move(sourceBuffer));

	return *this;
}

void Buffer::moveBufferContent(Buffer && sourceBuffer)
{
	m_data = std::move(sourceBuffer.m_data);

	m_bufferSize = sourceBuffer.m_bufferSize;
	sourceBuffer.m_data.resize(m_bufferSize);

	m_readIndex = sourceBuffer.m_readIndex;
	m_writeIndex = sourceBuffer.m_writeIndex;

	sourceBuffer.clearIndexes();
}

void Buffer::copyBufferContent(const Buffer & sourceBuffer)
{
	m_data = sourceBuffer.m_data;
	m_readIndex = sourceBuffer.m_readIndex;
	m_writeIndex = sourceBuffer.m_writeIndex;
	m_bufferSize = sourceBuffer.m_bufferSize;
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

void Buffer::write(const ByteArray & byteArray)
{
	std::copy(byteArray.cbegin(), byteArray.cend(), m_data.begin() + m_writeIndex);
	m_writeIndex += byteArray.size();
}

Byte_8 Buffer::read() const
{
	m_readIndex--;
	return m_data[m_readIndex];
}

ByteArray Buffer::read(const int & size) const
{
	ByteArray loadedData(size);

	std::copy(m_data.cbegin() + m_readIndex,
			  m_data.cbegin() + m_readIndex + size,
			  loadedData.begin());

	m_readIndex += size;

	return std::move(loadedData);
}

const srl::ByteArray & Buffer::data() const
{
	return m_data;
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
	m_data.resize(m_bufferSize);
}

} //end of namespace