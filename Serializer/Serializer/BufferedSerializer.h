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
					   const unsigned int & maxBufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~BufferedSerializer();

	BufferedSerializer(BufferedSerializer && serializer);
	BufferedSerializer & operator=(BufferedSerializer && serializer);

	BufferedSerializer(const BufferedSerializer & serializer) = delete;
	BufferedSerializer & operator=(const BufferedSerializer & serializer) = delete;

	virtual bool openFile(const Path & dir, const IOMode & mode = IOMode::Truncate) override;
	virtual void closeFile() override;

	virtual void clear() override;
	virtual bool isEmpty() const override;
	virtual operator bool() const override;

	virtual const ByteArray & getData() const override;

	virtual int getWriteIndex() const override;
	virtual int getReadIndex() const override;

	virtual bool setWriteIndex(const int & index) override;
	virtual bool setReadIndex(const int & index) const override;

	virtual Byte_8 & at(const unsigned int & index) override;
	virtual const Byte_8 & at(const unsigned int & index) const override;

	virtual BufferedSerializer & operator<<(const ISerializable & serializable) override;
	virtual const BufferedSerializer & operator>>(ISerializable & serializable) const override;

	//BufferedSerializer specific
	unsigned int getMaxBufferSize() const;
	unsigned int getBufferSize() const;
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
	const unsigned int MAX_BUFFER_SIZE;
	unsigned int validateProvidedBufferSize(const unsigned int & maxBufferSize);

	std::unique_ptr<IBuffer> m_buffer;
};

} //end of namespace