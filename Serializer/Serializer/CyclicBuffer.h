#pragma once
#include "ICyclicBuffer.h"
#include <vector>
#include <memory>

namespace srl
{

class CyclicBuffer : public ICyclicBuffer
{
public:
	explicit CyclicBuffer(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN);
	virtual ~CyclicBuffer();

	CyclicBuffer(const CyclicBuffer & sourceBuffer);
	CyclicBuffer(CyclicBuffer && sourceBuffer);

	CyclicBuffer & operator=(const CyclicBuffer & sourceBuffer);
	CyclicBuffer & operator=(CyclicBuffer && sourceBuffer);

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

	void moveBufferContent(CyclicBuffer && sourceBuffer);
	void copyBufferContent(const CyclicBuffer & sourceBuffer);

	void fitInternalBuffer();
	
	ByteArray m_data;
	mutable unsigned int m_index = 0;
};

} //end of namespace

