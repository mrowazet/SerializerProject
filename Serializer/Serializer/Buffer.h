#pragma once
#include "IBuffer.h"
#include <vector>
#include <memory>

namespace srl
{

class Buffer : public IBuffer
{
public:
	explicit Buffer(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~Buffer();

	Buffer(const Buffer & sourceBuffer);
	Buffer(Buffer && sourceBuffer);

	Buffer & operator=(const Buffer & sourceBuffer);
	Buffer & operator=(Buffer && sourceBuffer);

	virtual unsigned int size() const override;
	virtual bool isEmpty() const override;
	virtual operator bool() const override;
	virtual void clear() override;

	virtual void write(const Byte_8 & byte) override;
	virtual void write(const ByteArray & byteArray) override;

	virtual Byte_8 read() const override;
	virtual ByteArray read(const int& size) const override;

	virtual const srl::Byte_8 & data() const override;

protected:
	unsigned int BUFFER_SIZE; //TODO should be removed?

	void moveBufferContent(Buffer && sourceBuffer);
	void copyBufferContent(const Buffer & sourceBuffer);

	void fitInternalBuffer();
	
	ByteArray m_data;
	mutable unsigned int m_index = 0;
};

} //end of namespace

