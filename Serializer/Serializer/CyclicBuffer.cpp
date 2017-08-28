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
	//TODO add copy constructor
}

CyclicBuffer::CyclicBuffer(CyclicBuffer && sourceBuffer)
	:BUFFER_SIZE(sourceBuffer.BUFFER_SIZE)
{
	//TODO add move semantic constructor
}

CyclicBuffer & CyclicBuffer::operator=(const CyclicBuffer & sourceBuffer)
{	
	//TODO add copy assignment operator
	return *this;
}

CyclicBuffer & CyclicBuffer::operator=(CyclicBuffer && sourceBuffer)
{	
	//TODO add move semantic assignment operator
	return *this;
}

unsigned int CyclicBuffer::size() const
{
	return m_data.size();
}

bool CyclicBuffer::isEmpty() const
{
	return false; //TODO
}

CyclicBuffer::operator bool() const
{
	return isEmpty();
}

void CyclicBuffer::clear()
{
	//TODO
}

const srl::Byte_8 & CyclicBuffer::data() const
{
	return m_data[0];
}

} //end of namespace