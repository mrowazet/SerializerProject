#pragma once
#include "BufferedSerializer.h"
#include "CyclicBufferMock.h"

namespace srl
{

class BufferedSerializerTestable : public BufferedSerializer
{
public:
	BufferedSerializerTestable(const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializerTestable();

	CyclicBufferMock & getCyclicBufferMock();
	void clearmo() { m_buffer.reset(); };

};

} //end of namespace

