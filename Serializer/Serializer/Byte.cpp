#include "Byte.h"
#include <iostream>
#include <climits>

bool operator==(const std::string & byteAsString, const srl::Byte_8 & byte)
{
	return byte == byteAsString;
}

bool operator==(const char * byteAsCharArray, const srl::Byte_8 & byte)
{
	return byte == byteAsCharArray;
}

bool operator!=(const std::string & byteAsString, const srl::Byte_8 & byte)
{
	return byte != byteAsString;
}

bool operator!=(const char * byteAsCharArray, const srl::Byte_8 & byte)
{
	return byte != byteAsCharArray;
}

namespace srl
{

Byte_8::Byte_8()
	:bit0(false),
	 bit1(false),
	 bit2(false),
	 bit3(false),
	 bit4(false),
	 bit5(false),
	 bit6(false),
	 bit7(false)
{

}

Byte_8::Byte_8(const std::string & byteAsString)
{
	clear();

	if (byteAsString.size() == NR_OF_BITS && isContentCorrect(byteAsString))
		parseByteFromString(byteAsString);
}

Byte_8::Byte_8(const char * byteAsCharArray)
{
	clear();
	std::string content(byteAsCharArray, NR_OF_BITS);

	if (content.size() == NR_OF_BITS && isContentCorrect(content))
		parseByteFromString(content);
}

Byte_8::Byte_8(const char & character)
{
	char * thisByte = reinterpret_cast<char*>(&(*this));
	*thisByte = character;
}

Byte_8::~Byte_8()
{
}

bool Byte_8::isContentCorrect(const std::string & byteAsString) const
{
	for (int i = 0; i < NR_OF_BITS - 1; i++)
	{
		if (byteAsString[i] != '0' && byteAsString[i] != '1')
			return false;		
	}

	return true;
}

void Byte_8::parseByteFromString(const std::string & byteAsString)
{
	byteAsString[0] == '0' ? bit7 = false : bit7 = true;
	byteAsString[1] == '0' ? bit6 = false : bit6 = true;
	byteAsString[2] == '0' ? bit5 = false : bit5 = true;
	byteAsString[3] == '0' ? bit4 = false : bit4 = true;
	byteAsString[4] == '0' ? bit3 = false : bit3 = true;
	byteAsString[5] == '0' ? bit2 = false : bit2 = true;
	byteAsString[6] == '0' ? bit1 = false : bit1 = true;
	byteAsString[7] == '0' ? bit0 = false : bit0 = true;
}

std::string Byte_8::getAsString() const
{
	std::string bits(Byte_8::NR_OF_BITS, '0');

	bit7 ? bits[0] = '1' : bits[0] = '0';
	bit6 ? bits[1] = '1' : bits[1] = '0';
	bit5 ? bits[2] = '1' : bits[2] = '0';
	bit4 ? bits[3] = '1' : bits[3] = '0';
	bit3 ? bits[4] = '1' : bits[4] = '0';
	bit2 ? bits[5] = '1' : bits[5] = '0';
	bit1 ? bits[6] = '1' : bits[6] = '0';
	bit0 ? bits[7] = '1' : bits[7] = '0';
	
	return bits;
}

std::vector<bool> Byte_8::getAsBoolVector() const
{
	std::vector<bool> bits;
	bits.resize(NR_OF_BITS);

	bit0 ? bits[0] = true : bits[0] = false;
	bit1 ? bits[1] = true : bits[1] = false;
	bit2 ? bits[2] = true : bits[2] = false;
	bit3 ? bits[3] = true : bits[3] = false;
	bit4 ? bits[4] = true : bits[4] = false;
	bit5 ? bits[5] = true : bits[5] = false;
	bit6 ? bits[6] = true : bits[6] = false;
	bit7 ? bits[7] = true : bits[7] = false;

	return bits;
}

std::vector<int> Byte_8::getAsIntVector() const
{
	std::vector<int> bits;
	bits.resize(NR_OF_BITS);

	bit0 ? bits[0] = 1 : bits[0] = 0;
	bit1 ? bits[1] = 1 : bits[1] = 0;
	bit2 ? bits[2] = 1 : bits[2] = 0;
	bit3 ? bits[3] = 1 : bits[3] = 0;
	bit4 ? bits[4] = 1 : bits[4] = 0;
	bit5 ? bits[5] = 1 : bits[5] = 0;
	bit6 ? bits[6] = 1 : bits[6] = 0;
	bit7 ? bits[7] = 1 : bits[7] = 0;

	return bits;
}

bool Byte_8::getBit(const int & index) const
{
	if (index > NR_OF_BITS - 1 || index < 0)
	{
		return false;
	}

	if (index == 0) return bit0;
	if (index == 1) return bit1;
	if (index == 2) return bit2;
	if (index == 3) return bit3;
	if (index == 4) return bit4;
	if (index == 5) return bit5;
	if (index == 6) return bit6;
	if (index == 7) return bit7;
}

bool Byte_8::setBit(const int & index, const bool & isActive)
{
	if (index > NR_OF_BITS - 1 || index < 0)
	{
		return false ;
	}

	if (index == 0) { bit0 = isActive; return true; }
	if (index == 1) { bit1 = isActive; return true; }
	if (index == 2) { bit2 = isActive; return true; }
	if (index == 3) { bit3 = isActive; return true; }
	if (index == 4) { bit4 = isActive; return true; }
	if (index == 5) { bit5 = isActive; return true; }
	if (index == 6) { bit6 = isActive; return true; }
	if (index == 7) { bit7 = isActive; return true; }
}

bool Byte_8::isEmpty() const
{
	if (bit0 || bit1 || bit2 || bit3 || bit4 || bit5 || bit6 || bit7)
		return false;
	else
		return true;
}

void Byte_8::clear()
{
	bit0 = false;
	bit1 = false;
	bit2 = false;
	bit3 = false;
	bit4 = false;
	bit5 = false;
	bit6 = false;
	bit7 = false;
}

bool Byte_8::operator==(const Byte_8 & byte) const
{
	if (bit0 != byte.bit0 || bit1 != byte.bit1 || bit2 != byte.bit2 || bit3 != byte.bit3 ||
		bit4 != byte.bit4 || bit5 != byte.bit5 || bit6 != byte.bit6 || bit7 != byte.bit7  )
		return false;
	else
		return true;
}

bool Byte_8::operator==(const std::string & byteAsString) const
{
	if (byteAsString.size() == NR_OF_BITS && isContentCorrect(byteAsString))
	{
		Byte_8 byte(byteAsString);
		return *this == byte;
	}
	else
	{
		return false;
	}
}

bool Byte_8::operator==(const char * byteAsCharArray) const
{
	std::string content(byteAsCharArray);

	if (content.size() == NR_OF_BITS && isContentCorrect(content))
	{
		Byte_8 byte(content);
		return *this == byte;
	}
	else
	{
		return false;
	}
}

bool Byte_8::operator!=(const Byte_8 & byte) const
{
	return !(*this == byte);
}

bool Byte_8::operator!=(const std::string & byteAsString) const
{
	if (byteAsString.size() == NR_OF_BITS && isContentCorrect(byteAsString))
	{
		Byte_8 byte(byteAsString);
		return *this != byte;
	}
	else
	{
		return true;
	}
}

bool Byte_8::operator!=(const char * byteAsCharArray) const
{
	std::string content(byteAsCharArray);

	if (content.size() == NR_OF_BITS && isContentCorrect(content))
	{
		Byte_8 byte(content);
		return *this != byte;
	}
	else
	{
		return true;
	}
}

} //end of namespace