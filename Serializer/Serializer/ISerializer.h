#pragma once
#include <iostream>
#include <string>
#include "Constants.h"
#include "Byte.h"
#include "ByteArray.h"
#include "ISerializable.h"

namespace srl
{

class ISerializer
{
public:
	ISerializer() = default;
	virtual ~ISerializer() = default;

	ISerializer(ISerializer && serializer) = default;
	ISerializer & operator=(ISerializer && serializer) = default;

	ISerializer(const ISerializer & serializer) = delete;
	ISerializer & operator=(const ISerializer & serializer) = delete;


	virtual void clear() = 0;
	virtual unsigned int size() const = 0;
	virtual bool isEmpty() const = 0;
	virtual operator bool() const = 0;

	virtual const ByteArray & getData() const = 0;

	virtual int getWriteIndex() const = 0;
	virtual int getReadIndex() const = 0;

	virtual bool setWriteIndex(const int & index) = 0;
	virtual bool setReadIndex(const int & index) const = 0;

	virtual Byte_8 & at(const unsigned int & index) = 0;
	virtual const Byte_8 & at(const unsigned int & index) const = 0;

	virtual ISerializer & operator<<(const ISerializable & serializable) = 0;
	virtual const ISerializer & operator>>(ISerializable & serializable) const = 0;
};

} //end of namespace
