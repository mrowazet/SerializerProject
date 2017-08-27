#include "Serializer.h"
#include <stdexcept>

namespace srl
{

Serializer::Serializer()
{

}

Serializer::Serializer(const ByteArray & byteArray)
{
	m_data = byteArray;
	setWriteIndex(bufferSize());
}

Serializer::Serializer(Serializer && serializer)
{
	moveSerializerContent(std::move(serializer));
}

Serializer::~Serializer()
{
}

void Serializer::clear()
{
	m_data.clear();

	m_writeIndex = 0;
	m_readIndex = 0;
}

unsigned int Serializer::size() const
{
	return m_data.size();
}

bool Serializer::isEmpty() const
{
	return m_data.isEmpty();
}

Serializer::operator bool()
{
	return m_data.isEmpty();
}

const ByteArray & Serializer::getData() const
{
	return m_data;
}

int Serializer::getWriteIndex() const
{
	return m_writeIndex;
}

int Serializer::getReadIndex() const
{
	return m_readIndex;
}

bool Serializer::setWriteIndex(const int & index)
{
	if (isWriteIndexCorrect(index))
	{
		m_writeIndex = index;
		return true;
	}
	else
	{
		return false;
	}
}

bool Serializer::setReadIndex(const int & index) const
{
	if (isReadIndexCorrect(index))
	{
		m_readIndex = index;
		return true;
	}
	else
	{
		return false;
	}
}
bool Serializer::isReadIndexCorrect(const unsigned int & index) const
{
	return index < bufferSize() && index >= 0;
}

bool Serializer::isWriteIndexCorrect(const unsigned int & index) const
{
	return index <= bufferSize() && index >= 0;
}

unsigned int Serializer::bufferSize() const
{
	return static_cast<unsigned int>(m_data.size());
}

Serializer & Serializer::operator=(Serializer && serializer)
{
	moveSerializerContent(std::move(serializer));
	return *this;
}

void Serializer::moveSerializerContent(Serializer && serializer)
{
	m_data = std::move(serializer.m_data);

	m_writeIndex = serializer.m_writeIndex;
	m_readIndex = serializer.m_readIndex;

	serializer.m_writeIndex = 0;
	serializer.m_readIndex = 0;
}

Byte_8 & Serializer::at(const unsigned int & index)
{
	if (index > bufferSize())
		throw std::out_of_range("Index out of range!");
	else
		return m_data[index];
}

const Byte_8 & Serializer::at(const unsigned int & index) const
{
	if (index > bufferSize())
		throw std::out_of_range("Index out of range!");
	else
		return m_data[index];
}

//Specialized operators
Serializer & Serializer::operator<<(const ByteArray & byteArray)
{
	const int ArraySize = byteArray.size();
	const int RequiredExtraBytes = (m_writeIndex + ArraySize) - bufferSize();

	if (RequiredExtraBytes > 0)
		m_data.resize(m_data.size() + RequiredExtraBytes);

	if (ArraySize > 0)
	{
		memcpy(&m_data[m_writeIndex], &byteArray[0], ArraySize);
		m_writeIndex += ArraySize;
	}

	return *this;
}

Serializer & Serializer::operator<<(const std::string & value)
{	
	*this << value.c_str();
	return *this;
}

Serializer & Serializer::operator<<(const char * c_str)
{
	const int Size = static_cast<int>(std::strlen(c_str));
	
	const int RequiredExtraBytes = (m_writeIndex + sizeof(int) + Size) - bufferSize();
	m_data.resize(m_data.size() + RequiredExtraBytes);

	*this << Size;

	if (Size > 0)
	{
		memcpy(&m_data[m_writeIndex], c_str, Size);
		m_writeIndex += Size;
	}

	return *this;
}

const Serializer & Serializer::operator>>(std::string & value) const
{
	int stringSize = -1;
	*this >> stringSize;

	if (stringSize == -1) //invalid
		return *this;

	if (stringSize == 0) //empty string
	{
		value.clear();
		return *this;
	}
		
	if (m_readIndex + stringSize > bufferSize()) //stringSize indicates too long string
	{
		m_readIndex -= sizeof(int);
		return *this;
	}
	else
	{
		value.resize(stringSize);
		memcpy(&value[0], &m_data[m_readIndex], stringSize);
		m_readIndex += stringSize;

		return *this;
	}
}

Serializer & Serializer::operator<<(const ISerializable & serializable)
{
	*this << serializable.getAsString();
	return *this;
}

const Serializer & Serializer::operator>>(ISerializable & serializable) const
{
	std::string data;
	*this >> data;

	if(!data.empty())
		serializable.initFromString(data);

	return *this;
}

} //end of namespace
