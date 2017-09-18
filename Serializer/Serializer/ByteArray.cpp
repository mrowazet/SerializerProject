#include "ByteArray.h"
#include <iostream>
#include <algorithm>

namespace srl
{

ByteArray::ByteArray(const unsigned int & sizeInBytes)
{
	m_data.resize(sizeInBytes);
}

ByteArray::ByteArray(const ByteArray & byteArray)
{
	m_data = byteArray.m_data;
}

ByteArray::ByteArray(ByteArray && byteArray)
{
	m_data = std::move(byteArray.m_data);
}

ByteArray::~ByteArray()
{
}

std::string ByteArray::getAsString() const
{
	static const int BITS = Byte_8::BITS_IN_BYTE;
	const int BufferSize = m_data.size();

	std::string output;
	output.resize(BufferSize * BITS);

	for (int i = 0, writePos = 0; i < BufferSize; i++, writePos += BITS)
	{
		output.replace(writePos, BITS, m_data[i].getAsString());
	}
				
	return output;
}

unsigned int ByteArray::size() const
{
	return static_cast<int>(m_data.size());
}

void ByteArray::clear()
{
	m_data.clear();
}

void ByteArray::resize(const unsigned int & sizeInBytes)
{
	resizeBuffer(sizeInBytes);
}

void ByteArray::clearBuffer()
{
	m_data.clear();
}

void ByteArray::resizeBuffer(const unsigned int & newBufferSize)
{
	m_data.resize(newBufferSize);
}

bool ByteArray::isZeroed() const
{
	if (size() == 0)
		return true;

	for (auto & byte : m_data)
	{
		if (!byte.isEmpty())
			return false;
	}

	return true;
}

bool ByteArray::isEmpty() const
{
	return m_data.empty();
}

bool ByteArray::crop(const unsigned int & headBytes, const unsigned int & tailBytes)
{
	if(headBytes == 0 && tailBytes == 0)
		return false;

	const unsigned int Size = size();

	if (headBytes >= Size || tailBytes >= Size || (headBytes + tailBytes) >= Size)
	{
		clear();
	}
	else
	{
		m_data.erase(m_data.begin(), m_data.begin() + headBytes);
		m_data.erase(m_data.end() - tailBytes, m_data.end());
	}

	return true;
}

bool ByteArray::cropHead(const unsigned int & bytes)
{
	return crop(bytes, 0);
}

bool ByteArray::cropTail(const unsigned int & bytes)
{
	return crop(0, bytes);
}

ByteArray ByteArray::slice(const unsigned int & headIndex, const unsigned int & tailIndex) const
{
	const unsigned int Size = size();

	if (headIndex > Size || tailIndex > Size)
	{
		return ByteArray(0);
	}
		
	if (headIndex == tailIndex)
	{
		ByteArray newArray(1);
		newArray.setByte(getByte(headIndex), 0);
		return newArray; 
	}

	if (tailIndex > headIndex) //wrong order case
		return bufferSlice(headIndex, tailIndex);
	else
		return bufferSlice(tailIndex, headIndex);
}

void ByteArray::fillWithZeroes()
{
	std::fill(m_data.begin(), m_data.end(), 0);
}

ByteArray ByteArray::bufferSlice(const unsigned int & headIndex, const unsigned int & tailIndex) const
{
	const int NeededSize = tailIndex - headIndex + 1; // +1 because array should contain indicated indexes
	ByteArray newArray(NeededSize);

	for (unsigned int i = headIndex, j = 0; i <= tailIndex; i++, j++)
		newArray.setByte(m_data[i], j);
	
	return newArray;
}

Byte_8 ByteArray::getByte(const unsigned int & index) const
{
	if (index < size())
	{
		return m_data[index];
	}
	else
	{
		return Byte_8();
	}
}

bool ByteArray::setByte(const Byte_8 & byte, const unsigned int & index)
{
	if (index < size())
	{
		m_data[index] = byte;
		return true;
	}
	else
	{
		return false;
	}
}

ByteArray ByteArray::operator+(const ByteArray & byteArray) const
{
	ByteArray newArray = *this;
	newArray += byteArray;

	return newArray;
}

ByteArray ByteArray::operator+(const Byte_8 & byte) const
{
	ByteArray newArray = *this;
	newArray += byte;

	return newArray;
}

ByteArray & ByteArray::operator=(const ByteArray & byteArray)
{
	m_data = byteArray.m_data;
	return *this;
}

ByteArray & ByteArray::operator=(ByteArray && byteArray)
{
	m_data = std::move(byteArray.m_data);
	return *this;
}

ByteArray & ByteArray::operator+=(const ByteArray & byteArray)
{
	const int ByteArraySize = byteArray.size();

	if (ByteArraySize == 0)
		return *this;

	const unsigned int PrevSize = size();
	const unsigned int NewSize = PrevSize + byteArray.size();

	resizeBuffer(NewSize);
	memcpy(&m_data[PrevSize], &byteArray.m_data[0], PrevSize);

	return *this;
}

ByteArray & ByteArray::operator+=(const Byte_8 & byte)
{
	const unsigned int PrevSize = size();
	const unsigned int NewSize = PrevSize + 1;

	resizeBuffer(NewSize);
	m_data[PrevSize] = byte;

	return *this;
}

ByteArray & ByteArray::operator<<(const Byte_8 & byte)
{
	*this += byte;

	return *this;
}

bool ByteArray::operator==(const ByteArray & byteArray) const
{
	if(size() != byteArray.size())
		return false;

	for (unsigned int i = 0; i < size(); i++)
	{
		if (m_data[i] != byteArray.m_data[i])
			return false;
	}

	return true;
}

bool ByteArray::operator!=(const ByteArray & byteArray) const
{
	return !(*this == byteArray);
}

Byte_8 & ByteArray::operator[](const int & index)
{
	return m_data[index];
}

const Byte_8 & ByteArray::operator[](const int & index) const
{
	return m_data[index];
}

} //end of namespace

srl::ByteArray operator+(const srl::Byte_8 & byte, const srl::ByteArray & byteArray)
{
	srl::ByteArray newArray(0);
	newArray += byte;
	newArray += byteArray;

	return newArray;
}