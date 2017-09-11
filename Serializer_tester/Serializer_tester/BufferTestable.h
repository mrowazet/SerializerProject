#pragma once
#include "Buffer.h"

namespace srl
{

class BufferTestable : public Buffer
{
public:
	BufferTestable(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN)
		:Buffer(bufferSize),
		 internalData(m_data)
	{

	}

	BufferTestable(const BufferTestable & sourceBuffer)
		:Buffer(sourceBuffer),
		 internalData(m_data)
	{

	}

	BufferTestable(BufferTestable && sourceBuffer)
		:Buffer(std::move(sourceBuffer)),
		internalData(m_data)
	{

	}

	BufferTestable & operator=(const BufferTestable & sourceBuffer)
	{
		Buffer::operator=(sourceBuffer);
		return *this;
	}

	BufferTestable & operator=(BufferTestable && sourceBuffer)
	{
		Buffer::operator=(std::move(sourceBuffer));
		return *this;
	}

	using Buffer::m_readIndex;
	using Buffer::m_writeIndex;
	ByteArray& internalData;
};

} //end of namespace