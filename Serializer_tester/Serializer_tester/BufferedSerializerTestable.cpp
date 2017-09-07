#include "BufferedSerializerTestable.h"

namespace srl
{
	using namespace testing;

BufferedSerializerTestable::BufferedSerializerTestable(const unsigned int & maxBufferSize)
	: BufferedSerializer(maxBufferSize)
{
	m_buffer = std::make_unique<testing::StrictMock<BufferMock>>();
}

BufferedSerializerTestable::~BufferedSerializerTestable()
{

}

BufferMock & BufferedSerializerTestable::getCyclicBufferMock()
{
	return dynamic_cast<BufferMock&>(*m_buffer);
}

} //end of namespace