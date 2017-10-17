#pragma once
#include "ActiveSerializer.h"
#include "Buffer.h"
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
	unsigned int getBufferSize() const; //TODO probably should be removed
	void clearBuffer();

	//TODO
	//flushBuffer
	//getBuffer
	//reloadBuffer
	//setBufferWriteIndex
	//setBufferReadIndex
	//getBufferWriteIndex
	//getBufferReadIndex
	//open after open situation

protected:
	void moveBufferedSerializerContent(BufferedSerializer && serializer);
	void initBuffer(const unsigned int & bufferSize);

	virtual void writeToFile(const char* data, const unsigned int size);
	virtual void readFromFile(const unsigned int size);

	std::unique_ptr<IBuffer> m_buffer;

	using ActiveSerializer::openFileForRead;
	using ActiveSerializer::openFileForWrite;
	using ActiveSerializer::openFileForReadAndWrite;
};

} //end of namespace