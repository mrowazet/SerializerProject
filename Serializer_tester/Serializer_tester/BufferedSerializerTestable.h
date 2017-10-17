#pragma once
#include "BufferedSerializer.h"
#include "BufferMock.h"

namespace srl
{

class IFileHandling
{
public:
	virtual void writeToFile(const char* data, const unsigned int size) = 0;
	virtual void readFromFile(const unsigned int size) = 0;
};

class FileHandlingMock : public IFileHandling
{
public:
	MOCK_METHOD2(writeToFile, void(const char*, const unsigned int));
	MOCK_METHOD1(readFromFile, void(const unsigned int));
};

class BufferedSerializerTestable : public BufferedSerializer
{
public:
	BufferedSerializerTestable(const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializerTestable();

	BufferedSerializerTestable(BufferedSerializerTestable&&) = default;
	BufferedSerializerTestable & operator=(BufferedSerializerTestable&&) = default;

	BufferMock& getBufferMock();
	FileHandlingMock& getFileHandlingMock();

protected:
	virtual void writeToFile(const char* data, const unsigned int size);
	virtual void readFromFile(const unsigned int size);

	std::unique_ptr<FileHandlingMock> m_fileHandlingMock;
};

} //end of namespace

