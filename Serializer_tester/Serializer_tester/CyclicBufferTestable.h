#pragma once
#include "CyclicBuffer.h"

namespace srl
{

class CyclicBufferTestable : public CyclicBuffer
{
public:
	CyclicBufferTestable(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN)
		:CyclicBuffer(bufferSize),
		 internalData(m_data)
	{

	}

	using CyclicBuffer::m_index;
	ByteArray& internalData;
};

} //end of namespace