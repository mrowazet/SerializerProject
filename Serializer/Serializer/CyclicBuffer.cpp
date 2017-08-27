#include "CyclicBuffer.h"

namespace srl
{

CyclicBuffer::CyclicBuffer(const unsigned int & bufferSize)
	:BUFFER_SIZE(bufferSize)
{
	m_data.resize(BUFFER_SIZE);
}

CyclicBuffer::~CyclicBuffer()
{
}

CyclicBuffer::CyclicBuffer(const CyclicBuffer & sourceBuffer)
	:BUFFER_SIZE(sourceBuffer.BUFFER_SIZE)
{
	//TODO
}

CyclicBuffer::CyclicBuffer(CyclicBuffer && sourceBuffer)
	:BUFFER_SIZE(sourceBuffer.BUFFER_SIZE)
{
	//TODO
}

CyclicBuffer & CyclicBuffer::operator=(const CyclicBuffer & sourceBuffer)
{	
	//TODO
	return *this;
}

CyclicBuffer & CyclicBuffer::operator=(CyclicBuffer && sourceBuffer)
{	
	//TODO
	return *this;
}

unsigned int CyclicBuffer::getBufferSize() const
{
	return m_data.size();
}

} //end of namespace