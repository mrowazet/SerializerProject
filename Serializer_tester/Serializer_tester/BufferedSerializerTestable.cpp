#include "BufferedSerializerTestable.h"

namespace srl
{
	using namespace testing;

BufferedSerializerTestable::BufferedSerializerTestable(const unsigned int & maxBufferSize)
	: BufferedSerializer(maxBufferSize)
{
	m_buffer = std::make_unique<testing::StrictMock<BufferMock>>();
	m_fileHandlingMock = std::make_unique<testing::StrictMock<FileHandlingMock>>();
}

BufferedSerializerTestable::~BufferedSerializerTestable()
{

}

BufferMock & BufferedSerializerTestable::getBufferMock()
{
	return dynamic_cast<BufferMock&>(*m_buffer);
}

FileHandlingMock & BufferedSerializerTestable::getFileHandlingMock()
{
	return dynamic_cast<FileHandlingMock&>(*m_fileHandlingMock);
}

void BufferedSerializerTestable::writeToFile(const char* data, const unsigned int size)
{
	m_fileHandlingMock->writeToFile(data, size);
}

void BufferedSerializerTestable::readFromFile(const unsigned int size)
{
	m_fileHandlingMock->readFromFile(size);
}

} //end of namespace