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
	explicit CyclicBuffer(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN);
	~CyclicBuffer();

	CyclicBuffer(const CyclicBuffer & sourceBuffer);
	CyclicBuffer(CyclicBuffer && sourceBuffer);

	CyclicBuffer & operator=(const CyclicBuffer & sourceBuffer);
	CyclicBuffer & operator=(CyclicBuffer && sourceBuffer);

	unsigned int size() const;
	bool isEmpty() const;
	operator bool() const;
	void clear();

	inline const auto & data() const { return m_data[0]; }

protected:
	const unsigned int BUFFER_SIZE;

	ByteArray m_data;
};

} //end of namespace

