#pragma once
#include "Constants.h"
#include "Byte.h"
#include "ByteArray.h"

namespace srl
{

class ICyclicBuffer
{
public:
	ICyclicBuffer() = default;
	virtual ~ICyclicBuffer() = default;

	ICyclicBuffer(const ICyclicBuffer&) = default;
	ICyclicBuffer(ICyclicBuffer&&) = default;

	ICyclicBuffer& operator=(const ICyclicBuffer&) = default;
	ICyclicBuffer& operator=(ICyclicBuffer&&) = default;

	virtual unsigned int size() const = 0;
	virtual bool isEmpty() const = 0;
	virtual operator bool() const = 0;
	virtual void clear() = 0;

	virtual void write(const Byte_8 & byte) = 0;
	virtual void write(const ByteArray & byteArray) = 0;

	virtual Byte_8 read() const = 0;
	virtual ByteArray read(const int& size) const = 0;

	virtual const srl::Byte_8 & data() const = 0;

	//TODO
	/* 
		Performance improvement
		function operator[] can be added to return ptr to memory
		then we can write directly in internal buffer
	*/
};

} //end of namespace