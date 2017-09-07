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

	using Buffer::m_index;
	ByteArray& internalData;
};

} //end of namespace