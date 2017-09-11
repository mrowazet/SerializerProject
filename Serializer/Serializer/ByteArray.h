#pragma once
#include "Byte.h"
#include <vector>
#include <string>

namespace srl
{

class ByteArray
{
	public:
		ByteArray(const unsigned int & sizeInBytes = 0);
		ByteArray(const ByteArray & byteArray);
		ByteArray(ByteArray && byteArray);
		virtual ~ByteArray();

		std::string getAsString() const;
		unsigned int size() const;
		void clear();
		void resize(const unsigned int & sizeInBytes);
		bool isEmpty() const;
		bool isZeroed() const;

		bool crop(const unsigned int & headBytes, const unsigned int & tailBytes);
		bool cropHead(const unsigned int & bytes);
		bool cropTail(const unsigned int & bytes);

		ByteArray slice(const unsigned int & headIndex, const unsigned int & tailIndex) const;

		void fillWithZeroes();

		Byte_8 getByte(const unsigned int & index) const;
		bool setByte(const Byte_8 & byte, const unsigned int & index);

		Byte_8 & operator[](const int & index);
		const Byte_8 & operator[](const int & index) const;

		ByteArray operator+(const ByteArray & byteArray) const;
		ByteArray operator+(const Byte_8 & byte) const;

		ByteArray & operator=(const ByteArray & byteArray);
		ByteArray & operator=(ByteArray && byteArray);

		ByteArray & operator+=(const ByteArray & byteArray);
		ByteArray & operator+=(const Byte_8 & byte);

		ByteArray & operator<<(const Byte_8 & byte);

		bool operator==(const ByteArray & byteArray) const;
		bool operator!=(const ByteArray & byteArray) const;

	private:
		std::vector<Byte_8> m_data;

		void clearBuffer();
		void resizeBuffer(const unsigned int & newBufferSize);
		ByteArray bufferSlice(const unsigned int & headIndex, const unsigned int & tailIndex) const;
};

} //end of namespace

srl::ByteArray operator+(const srl::Byte_8 & byte, const srl::ByteArray & byteArray);

