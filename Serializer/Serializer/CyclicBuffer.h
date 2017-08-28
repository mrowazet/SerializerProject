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

	virtual const srl::Byte_8 & data() const override;

protected:
	const unsigned int BUFFER_SIZE;

	ByteArray m_data;
};

} //end of namespace

