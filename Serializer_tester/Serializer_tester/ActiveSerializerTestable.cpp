#include "ActiveSerializerTestable.h"

namespace srl
{

ActiveSerializerTestable::ActiveSerializerTestable()
{
}


ActiveSerializerTestable::~ActiveSerializerTestable()
{

}

bool ActiveSerializerTestable::openFile(const Path & dir, const IOMode & mode)
{
	return ActiveSerializer::openFileBase(dir, mode);
}

void ActiveSerializerTestable::closeFile()
{
	ActiveSerializer::closeFileBase();
}

void ActiveSerializerTestable::clear()
{
	ActiveSerializer::clearBase();
}

unsigned int ActiveSerializerTestable::size() const
{
	return 0;
}

bool ActiveSerializerTestable::isEmpty() const
{
	return false;
}

ActiveSerializerTestable::operator bool() const
{
	return false;
}

const ByteArray & ActiveSerializerTestable::getData() const
{
	return ByteArray();
}

int ActiveSerializerTestable::getWriteIndex() const
{
	return 0;
}

int ActiveSerializerTestable::getReadIndex() const
{
	return 0;
}

bool ActiveSerializerTestable::setWriteIndex(const int & index)
{
	return false;
}
bool ActiveSerializerTestable::setReadIndex(const int & index) const
{
	return false;
}

Byte_8 & ActiveSerializerTestable::at(const unsigned int & index)
{
	return Byte_8();
}

const Byte_8 & ActiveSerializerTestable::at(const unsigned int & index) const
{
	return Byte_8();
}

ActiveSerializerTestable & ActiveSerializerTestable::operator<<(const ByteArray & byteArray)
{
	return *this;
}

ActiveSerializerTestable & ActiveSerializerTestable::operator<<(const char * c_str)
{
	return *this;
}

ActiveSerializerTestable & ActiveSerializerTestable::operator<<(const std::string & value)
{
	return *this;
}

const ActiveSerializerTestable & ActiveSerializerTestable::operator>>(std::string & value) const
{
	return *this;
}

ActiveSerializerTestable & ActiveSerializerTestable::operator<<(const ISerializable & serializable)
{
	return *this;
}

const ActiveSerializerTestable & ActiveSerializerTestable::operator >> (ISerializable & serializable) const
{
	return *this;
}

} //end of namespace