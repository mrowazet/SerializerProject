#pragma once
#include <vector>
#include <string>
#include <bitset>

namespace srl
{

class Byte_8
{
	public:
		static constexpr int BITS_IN_BYTE = 8;

		Byte_8();
		Byte_8(const std::string & byteAsString);
		Byte_8(const char * byteAsCharArray);
		Byte_8(const char & character);

		Byte_8(const Byte_8 & byte_8) = default;
		~Byte_8();

		std::string getAsString() const;
		std::vector<bool> getAsBoolVector() const;
		std::vector<int> getAsIntVector() const;

		bool getBit(const int & index) const;
		bool setBit(const int & index, const bool & isActive = true);

		bool isEmpty() const;
		void clear();

		bool operator==(const Byte_8 & byte) const;
		bool operator==(const std::string & byteAsString) const;
		bool operator==(const char * byteAsCharArray) const;

		bool operator!=(const Byte_8 & byte) const;
		bool operator!=(const std::string & byteAsString) const;
		bool operator!=(const char * byteAsCharArray) const;

	private:

		bool isContentCorrect(const std::string & byteAsString) const;
		void parseByteFromString(const std::string & byteAsString);

		//Bit map
		bool bit0 : 1;
		bool bit1 : 1;
		bool bit2 : 1;
		bool bit3 : 1;
		bool bit4 : 1;
		bool bit5 : 1;
		bool bit6 : 1;
		bool bit7 : 1;
};

} //end of namespace

bool operator==(const std::string & byteAsString, const srl::Byte_8 & byte);
bool operator==(const char * byteAsCharArray, const srl::Byte_8 & byte);
bool operator!=(const std::string & byteAsString, const srl::Byte_8 & byte);
bool operator!=(const char * byteAsCharArray, const srl::Byte_8 & byte);

