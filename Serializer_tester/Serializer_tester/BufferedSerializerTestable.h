#pragma once
#include "BufferedSerializer.h"
#include "BufferMock.h"

namespace srl
{

class BufferedSerializerTestable : public BufferedSerializer
{
public:
	BufferedSerializerTestable(const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializerTestable();

	BufferMock & getBufferMock();
};

} //end of namespace

