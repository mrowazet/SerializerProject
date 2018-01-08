#include "BufferedSerializer.h"

namespace srl
{

namespace fs = std::experimental::filesystem;

BufferedSerializer::BufferedSerializer(const unsigned int & bufferSize)
{
	initBuffer(bufferSize);
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

void BufferedSerializer::writeToFile(const char * data, const unsigned int size)
{
	m_file->write(data, size);
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

unsigned int BufferedSerializer::getMaxBufferSize() const
{
	return m_buffer->size();
}

unsigned int BufferedSerializer::getLastCorrectBufferReadIndex() const
{
	return m_bufferedDataInfo.getLastCorrectReadIndex();
}

unsigned int BufferedSerializer::getLastCorrectBufferWriteIndex() const
{
	return m_bufferedDataInfo.getLastCorrectWriteIndex();
}

void BufferedSerializer::clear()
{
	if(isFileOpened())
	{
		ActiveSerializer::clearBase();
		clearBuffer();
		clearBufferInfo();
		clearIndexes();
	}
}

void BufferedSerializer::clearBuffer()
{
	m_buffer->clear();
	m_bufferedDataInfo.clearAccessIndexes();
}

void BufferedSerializer::flush()
{
	if(isAnyDataToFlush())
		flushDataFromBuffer();
}

bool BufferedSerializer::isAnyDataToFlush() const
{
	auto sizeOfDataToFlush = m_writeIndex - m_bufferedDataInfo.getBeginIndexRelativelyToFile();
	return sizeOfDataToFlush > 0;
}

void BufferedSerializer::flushDataFromBuffer()
{
	auto sizeOfDataToFlush = m_writeIndex - m_bufferedDataInfo.getBeginIndexRelativelyToFile();

	auto firstIter = m_buffer->cbegin();
	auto dataBegin = reinterpret_cast<const char*>(&(*firstIter));

	writeToFile(dataBegin, sizeOfDataToFlush);

	m_bufferedDataInfo.clearAccessIndexes();
	m_bufferedDataInfo.setBeginIndexRelativelyToFile(m_writeIndex);
}

void BufferedSerializer::clearIndexes()
{
	m_readIndex = 0;
	m_writeIndex = 0;
}

void BufferedSerializer::clearBufferInfo()
{
	m_bufferedDataInfo.clear();
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
	return index > 0 && index <= getFileSize();
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
	return index > 0 && index < getFileSize();
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

	if (isDataGraterOrEqaulBuffer(byteArray.size()))
	{
		writeByteArrayGraterThanBuffer(byteArray);
	}
	else
	{
		if (isDataGraterOrEqualAvailableFreeSpaceInBuffer(byteArray.size()))
		{
			writeByteArrayGraterThanAvailableSpaceInBuffer(byteArray);
		}
		else
		{
			writeByteArrayLessThanAvailableSpaceInBuffer(byteArray);
		}
	}

	return *this;
}

void BufferedSerializer::writeByteArrayGraterThanBuffer(const ByteArray& data)
{
	if(isAnyDataToFlush())
		flushDataFromBuffer();

	writeToFile(getDataPtr(data[0]), data.size());

	m_writeIndex += data.size();

	m_bufferedDataInfo.clearAccessIndexes();
	m_bufferedDataInfo.setBeginIndexRelativelyToFile(m_writeIndex);
}

void BufferedSerializer::writeByteArrayGraterThanAvailableSpaceInBuffer(const ByteArray& data)
{
	flushDataFromBuffer();

	m_buffer->clearIndexes();
	m_buffer->write(data);

	m_writeIndex += data.size();
	m_bufferedDataInfo.updateAccessIndexesByAddedDataSize(data.size());
}

void BufferedSerializer::writeByteArrayLessThanAvailableSpaceInBuffer(const ByteArray& data)
{
	m_buffer->write(data);

	m_writeIndex += data.size();
	m_bufferedDataInfo.updateAccessIndexesByAddedDataSize(data.size());
}

bool BufferedSerializer::isDataGraterOrEqaulBuffer(const unsigned int dataSize) const
{
	return dataSize >= m_buffer->size();
}

bool BufferedSerializer::isDataGraterOrEqualAvailableFreeSpaceInBuffer(const unsigned int dataSize) const
{
	auto availableSpaceInBuffer = m_buffer->size() - m_bufferedDataInfo.getLastCorrectWriteIndex();
	return dataSize >= availableSpaceInBuffer;
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