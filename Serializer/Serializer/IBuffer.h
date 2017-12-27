#pragma once
#include "Constants.h"
#include "Byte.h"
#include "ByteArray.h"

namespace srl
{

/*This class is facade for ByteArray but with additional features*/
class IBuffer
{
public:
	using Iter = ByteArray::Iter;
	using CIter = ByteArray::CIter;

	IBuffer() = default;
	virtual ~IBuffer() = default;

	IBuffer(const IBuffer&) = default;
	IBuffer(IBuffer&&) = default;

	IBuffer& operator=(const IBuffer&) = default;
	IBuffer& operator=(IBuffer&&) = default;

	virtual unsigned int size() const = 0;
	virtual bool isEmpty() const = 0;
	virtual operator bool() const = 0;
	virtual void clear() = 0;
	virtual void clearIndexes() = 0;

	virtual void write(const Byte_8 & byte) = 0;
	virtual void write(const ByteArray & byteArray) = 0;

	virtual Byte_8 read() const = 0;
	virtual ByteArray read(const int& size) const = 0;

	virtual int getReadIndex() const = 0;
	virtual int getWriteIndex() const = 0;

	virtual void setReadIndex(const int & index) const = 0;
	virtual void setWriteIndex(const int & index) = 0;

	virtual const srl::ByteArray & data() const = 0;

	virtual Iter begin() = 0;
	virtual CIter begin() const = 0;
	virtual CIter cbegin() const = 0;

	virtual Iter end() = 0;
	virtual CIter end() const = 0;
	virtual CIter cend() const = 0;
};

} //end of namespace