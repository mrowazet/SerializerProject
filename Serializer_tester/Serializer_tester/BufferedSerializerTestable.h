#pragma once
#include "BufferedSerializer.h"
#include "BufferMock.h"

namespace srl
{

class IFileHandling
{
public:
	virtual void writeToFile(const char* data, const unsigned int size) = 0;
	virtual void readFromFile(const unsigned int size) const = 0;
	virtual uintmax_t getFileSize() const = 0;
};

class FileHandlingMock : public IFileHandling
{
public:
	MOCK_METHOD2(writeToFile, void(const char*, const unsigned int));
	MOCK_CONST_METHOD1(readFromFile, void(const unsigned int));
	MOCK_CONST_METHOD0(getFileSize, uintmax_t());
};

class BufferedSerializerTestable : public BufferedSerializer
{
public:
	BufferedSerializerTestable(const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializerTestable();

	BufferedSerializerTestable(BufferedSerializerTestable&&) = default;
	BufferedSerializerTestable & operator=(BufferedSerializerTestable&&) = default;

	virtual uintmax_t getFileSize() const override;

	BufferMock& getBufferMock();
	FileHandlingMock& getFileHandlingMock();
	BufferedDataInfo& getBufferDataInfo();

protected:
	virtual void writeToFile(const char* data, const unsigned int size);
	virtual void readFromFile(const unsigned int size);

	std::unique_ptr<FileHandlingMock> m_fileHandlingMock;
};

} //end of namespace

