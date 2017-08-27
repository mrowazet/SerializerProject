#include "FlySerializer.h"

namespace srl
{

FlySerializer::FlySerializer()
{
}

FlySerializer::FlySerializer(FlySerializer && serializer)
{

}

FlySerializer & FlySerializer::operator=(FlySerializer && serializer)
{
	return *this;
}

FlySerializer::~FlySerializer()
{
}

void FlySerializer::clear()
{
}

unsigned int FlySerializer::size() const
{
	return 0;
}

bool FlySerializer::isEmpty() const
{
	return false;
}

FlySerializer::operator bool() const
{
	return false;
}

const ByteArray & FlySerializer::getData() const
{
	return ByteArray();
}

int FlySerializer::getWriteIndex() const
{
	return 0;
}

int FlySerializer::getReadIndex() const
{
	return 0;
}

bool FlySerializer::setWriteIndex(const int & index)
{
	return false;
}

bool FlySerializer::setReadIndex(const int & index) const
{
	return false;
}

Byte_8 & FlySerializer::at(const unsigned int & index)
{
	return Byte_8();
}

const Byte_8 & FlySerializer::at(const unsigned int & index) const
{
	return Byte_8();
}

FlySerializer & FlySerializer::operator<<(const ISerializable & serializable)
{
	return *this;
}

const FlySerializer & FlySerializer::operator>>(ISerializable & serializable) const
{
	return *this;
}

} //end of namespace
