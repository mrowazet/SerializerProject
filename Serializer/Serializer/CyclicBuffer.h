#pragma once
#include "Constants.h"
#include "Byte.h"
#include "ByteArray.h"
#include <vector>
#include <memory>

namespace srl
{

class CyclicBuffer
{
public:
	explicit CyclicBuffer(const unsigned int & bufferSize);
	~CyclicBuffer();

	CyclicBuffer(const CyclicBuffer & sourceBuffer);
	CyclicBuffer(CyclicBuffer && sourceBuffer);

	CyclicBuffer & operator=(const CyclicBuffer & sourceBuffer);
	CyclicBuffer & operator=(CyclicBuffer && sourceBuffer);

	unsigned int getBufferSize() const;

private:
	const unsigned int BUFFER_SIZE;

	std::vector<Byte_8> m_data;
};

} //end of namespace

