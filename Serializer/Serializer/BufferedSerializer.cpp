#include "BufferedSerializer.h"

namespace srl
{

namespace fs = std::experimental::filesystem;

BufferedSerializer::BufferedSerializer(const unsigned int & bufferSize)
{
	initBuffer(bufferSize);
}

BufferedSerializer::BufferedSerializer(const Path & dir,
									   const IOMode & mode,
									   const unsigned int & bufferSize)
	: BufferedSerializer(bufferSize)
{
	ActiveSerializer::openFileBase(dir, mode);
}

BufferedSerializer::BufferedSerializer(BufferedSerializer && serializer)
{
	moveBufferedSerializerContent(std::move(serializer));
}

BufferedSerializer & BufferedSerializer::operator=(BufferedSerializer && serializer)
{
	if (this != &serializer)
		moveBufferedSerializerContent(std::move(serializer));

	return *this;
}

BufferedSerializer::~BufferedSerializer()
{
	ActiveSerializer::closeFileBase();
}

void BufferedSerializer::moveBufferedSerializerContent(BufferedSerializer && serializer)
{
	//TODO expand - move rest 
	static_cast<ActiveSerializer&>(*this) = std::move(serializer);
	m_buffer = std::move(serializer.m_buffer);
}

void BufferedSerializer::initBuffer(const unsigned int & bufferSize)
{
	auto size = bufferSize;

	if (bufferSize < SERIALIZER_BUFFER_MIN)
		size = SERIALIZER_BUFFER_MIN;

	if (bufferSize > SERIALIZER_BUFFER_MAX)
		size = SERIALIZER_BUFFER_MAX;

	m_buffer.reset(new Buffer(size)); //protected ctor used - can't use std::make_unique<T>
}

void BufferedSerializer::writeToFile(const char * data, const unsigned int size)
{
}

void BufferedSerializer::readFromFile(const unsigned int size) const
{
}

bool BufferedSerializer::openFile(const Path & dir, const IOMode & mode)
{
	return ActiveSerializer::openFileBase(dir, mode);
}

void BufferedSerializer::closeFile() //TODO data from buffer should be flushed, clearIndexes, clearBuffer
{
	ActiveSerializer::closeFileBase();
}

unsigned int BufferedSerializer::getBufferSize() const
{
	return m_buffer->size();
}

void BufferedSerializer::clear()
{
	if(isFileOpened())
	{
		ActiveSerializer::clearBase();
		clearBuffer();
		clearIndexes();
	}
}

void BufferedSerializer::clearBuffer()
{
	m_buffer->clear();
}

void BufferedSerializer::clearIndexes()
{
	m_readIndex = 0;
	m_writeIndex = 0;
}

unsigned int BufferedSerializer::size() const
{
	return 0;
}

bool BufferedSerializer::isEmpty() const
{
	if(!isFileOpened())
		return true;

	if (getFileSize() == 0 && m_buffer->isEmpty())
		return true;
	else
		return false;
}

BufferedSerializer::operator bool() const
{
	return isEmpty();
}

const ByteArray & BufferedSerializer::getData() const
{
	return m_buffer->data();
}

int BufferedSerializer::getWriteIndex() const
{
	return m_writeIndex;
}

int BufferedSerializer::getReadIndex() const
{
	return m_readIndex;
}

bool BufferedSerializer::setWriteIndex(const int & index)
{
	if (!isFileOpened() || !isWriteIndexCorrect(index))
	{
		return false;
	}
	else
	{
		m_writeIndex = index;
		return true;
	}
}

bool BufferedSerializer::isWriteIndexCorrect(const IndexPosition & index) const
{
	return index <= getFileSize() && index > 0;
}

bool BufferedSerializer::setReadIndex(const int & index) const
{
	if (!isFileOpened() || !isReadIndexCorrect(index))
	{
		return false;
	}
	else
	{
		m_readIndex = index;
		return true;
	}
}

bool BufferedSerializer::isReadIndexCorrect(const IndexPosition & index) const
{
	return index < getFileSize() && index > 0;
}

Byte_8 & BufferedSerializer::at(const unsigned int & index)
{
	return Byte_8();
}

const Byte_8 & BufferedSerializer::at(const unsigned int & index) const
{
	return Byte_8();
}

BufferedSerializer & BufferedSerializer::operator<<(const ByteArray & byteArray)
{
	ASSERT_FILE_OPENED();


	if (byteArray.size() < m_buffer->size())
	{
		m_buffer->write(byteArray);
		m_writeIndex += byteArray.size();
	}
	else
	{

	}

	return *this;
}

BufferedSerializer & BufferedSerializer::operator<<(const char * c_str)
{
	ASSERT_FILE_OPENED();
	return *this;
}

BufferedSerializer & BufferedSerializer::operator<<(const std::string & value)
{
	ASSERT_FILE_OPENED();
	return *this;
}

const BufferedSerializer & BufferedSerializer::operator>>(std::string & value) const
{
	ASSERT_FILE_OPENED();
	return *this;
}

BufferedSerializer & BufferedSerializer::operator<<(const ISerializable & serializable)
{
	ASSERT_FILE_OPENED();
	return *this;
}

const BufferedSerializer & BufferedSerializer::operator>>(ISerializable & serializable) const
{
	ASSERT_FILE_OPENED();
	return *this;
}

} //end of namespace