#pragma once
#include "ActiveSerializer.h"
#include "Buffer.h"
#include "BufferedDataInfo.h"
#include <memory>

namespace srl
{

class BufferedSerializer : public ActiveSerializer
{
public:
	BufferedSerializer(const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	BufferedSerializer(const Path & dir,
					   const IOMode & mode = IOMode::Truncate,
					   const unsigned int & ufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializer();

	BufferedSerializer(BufferedSerializer && serializer);
	BufferedSerializer & operator=(BufferedSerializer && serializer);

	BufferedSerializer(const BufferedSerializer & serializer) = delete;
	BufferedSerializer & operator=(const BufferedSerializer & serializer) = delete;

	virtual bool openFile(const Path & dir, const IOMode & mode = IOMode::Truncate) override;
	virtual void closeFile() override;

	virtual void clear() override;
	virtual unsigned int size() const override;
	virtual bool isEmpty() const override;
	virtual operator bool() const override;

	virtual const ByteArray & getData() const override;

	virtual int getWriteIndex() const override;
	virtual int getReadIndex() const override;

	virtual bool setWriteIndex(const int & index) override;
	virtual bool setReadIndex(const int & index) const override;

	virtual Byte_8 & at(const unsigned int & index) override;
	virtual const Byte_8 & at(const unsigned int & index) const override;

	virtual BufferedSerializer & operator<<(const ByteArray & byteArray) override;
	virtual BufferedSerializer & operator<<(const char * c_str) override;

	virtual BufferedSerializer & operator<<(const std::string & value) override;
	virtual const BufferedSerializer & operator>>(std::string & value) const override;

	virtual BufferedSerializer & operator<<(const ISerializable & serializable) override;
	virtual const BufferedSerializer & operator>>(ISerializable & serializable) const override;

	//BufferedSerializer specific
	unsigned int getMaxBufferSize() const;
	void clearBuffer();
	void flush();

	//TODO
	//reloadBuffer with selected data index as start
	//open after open situation

protected:
	using IndexPosition = unsigned int;

	mutable IndexPosition m_readIndex = 0;
	IndexPosition m_writeIndex = 0;

	std::unique_ptr<IBuffer> m_buffer;
	BufferedDataInfo m_bufferedDataInfo;

	void moveBufferedSerializerContent(BufferedSerializer && serializer);
	void initBuffer(const unsigned int & bufferSize);
	void clearIndexes();
	void clearBufferInfo();
	bool isAnyDataToFlush() const;
	void flushDataFromBuffer();

	bool isReadIndexCorrect(const IndexPosition & index) const;
	bool isWriteIndexCorrect(const IndexPosition & index) const;

	unsigned int getLastCorrectBufferReadIndex() const;
	unsigned int getLastCorrectBufferWriteIndex() const;

	virtual void writeToFile(const char* data, const unsigned int size);
	virtual void readFromFile(const unsigned int size) const;
	
	void writeByteArrayGraterThanBuffer(const ByteArray& data);
	void writeByteArrayGraterThanAvailableSpaceInBuffer(const ByteArray& data);
	void writeByteArrayLessThanAvailableSpaceInBuffer(const ByteArray& data);

	bool isDataGraterOrEqaulBuffer(const unsigned int dataSize) const;
	bool isDataGraterOrEqualAvailableFreeSpaceInBuffer(const unsigned int dataSize) const;

	template<typename DataType>
	const char* getDataPtr(const DataType& data)
	{
		return reinterpret_cast<const char*>(&data);
	}

	using ActiveSerializer::openFileForRead;
	using ActiveSerializer::openFileForWrite;
	using ActiveSerializer::openFileForReadAndWrite;
};

} //end of namespace