#include "BufferedSerializerTestable.h"

namespace srl
{
	using namespace testing;

BufferedSerializerTestable::BufferedSerializerTestable(const unsigned int & maxBufferSize)
	: BufferedSerializer(maxBufferSize)
{
	m_buffer = std::make_unique<BufferMock>();
	EXPECT_CALL(static_cast<BufferMock&>(*m_buffer), size()).WillRepeatedly(Return(maxBufferSize));

	m_fileHandlingMock = std::make_unique<testing::StrictMock<FileHandlingMock>>();
}

BufferedSerializerTestable::~BufferedSerializerTestable()
{

}

uintmax_t BufferedSerializerTestable::getFileSize() const
{
	return m_fileHandlingMock->getFileSize();
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