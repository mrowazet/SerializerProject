#include "BufferedSerializer.h"

namespace srl
{

namespace fs = std::experimental::filesystem;

BufferedSerializer::BufferedSerializer(const unsigned int & maxBufferSize)
	:MAX_BUFFER_SIZE(validateProvidedBufferSize(maxBufferSize))
{
	m_buffer.reset(new Buffer(MAX_BUFFER_SIZE));
}

BufferedSerializer::BufferedSerializer(const Path & dir,
									   const IOMode & mode,
									   const unsigned int & maxBufferSize)
	:MAX_BUFFER_SIZE(validateProvidedBufferSize(maxBufferSize))
{
	ActiveSerializer::openFileBase(dir, mode);
}

BufferedSerializer::BufferedSerializer(BufferedSerializer && serializer)
	:MAX_BUFFER_SIZE(serializer.MAX_BUFFER_SIZE)
{
	//TODO add move semantic for constructor
}

BufferedSerializer & BufferedSerializer::operator=(BufferedSerializer && serializer)
{
	//TODO add move semantic for assignment operator
	return *this;
}

BufferedSerializer::~BufferedSerializer()
{
	ActiveSerializer::closeFileBase();
}

bool BufferedSerializer::openFile(const Path & dir, const IOMode & mode)
{
	return ActiveSerializer::openFileBase(dir, mode);
}

void BufferedSerializer::closeFile() //TODO data from buffer should be flushed
{
	ActiveSerializer::closeFileBase();
}

unsigned int BufferedSerializer::validateProvidedBufferSize(const unsigned int & maxBufferSize)
{
	if (maxBufferSize < SERIALIZER_BUFFER_MIN)
		return SERIALIZER_BUFFER_MIN;

	if (maxBufferSize > SERIALIZER_BUFFER_MAX)
		return SERIALIZER_BUFFER_MAX;

	return maxBufferSize;
}

unsigned int BufferedSerializer::getMaxBufferSize() const
{
	return MAX_BUFFER_SIZE;
}

unsigned int BufferedSerializer::getBufferSize() const
{
	return m_buffer->size();
}

void BufferedSerializer::clearBuffer()
{
	m_buffer->clear();
}

void BufferedSerializer::clear()
{
	if(m_file)
	{
		ActiveSerializer::clearBase();
		clearBuffer();
	}
}

unsigned int BufferedSerializer::size() const
{
	return 0;
}

bool BufferedSerializer::isEmpty() const
{
	if(!m_file)
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
	return 0;
}

int BufferedSerializer::getReadIndex() const
{
	return 0;
}

bool BufferedSerializer::setWriteIndex(const int & index)
{
	return false;
}

bool BufferedSerializer::setReadIndex(const int & index) const
{
	return false;
}

Byte_8 & BufferedSerializer::at(const unsigned int & index)
{
	return Byte_8();
}

const Byte_8 & BufferedSerializer::at(const unsigned int & index) const
{
	return Byte_8();
}

BufferedSerializer & BufferedSerializer::operator<<(const ISerializable & serializable)
{
	return *this;
}

const BufferedSerializer & BufferedSerializer::operator>>(ISerializable & serializable) const
{
	return *this;
}

} //end of namespace