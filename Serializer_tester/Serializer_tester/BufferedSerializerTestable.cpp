#include "BufferedSerializerTestable.h"

namespace srl
{
	using namespace testing;

BufferedSerializerTestable::BufferedSerializerTestable(const unsigned int & maxBufferSize)
	: BufferedSerializer(maxBufferSize)
{
	m_buffer = std::make_unique<CyclicBufferMock>();
}

BufferedSerializerTestable::~BufferedSerializerTestable()
{

}

CyclicBufferMock & BufferedSerializerTestable::getCyclicBufferMock()
{
	return dynamic_cast<CyclicBufferMock&>(*m_buffer);
}

} //end of namespace