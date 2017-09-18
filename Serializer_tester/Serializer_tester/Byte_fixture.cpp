#include "Byte_fixture.h"

using namespace ::srl;

Byte_fixture::Byte_fixture()
{
}

Byte_fixture::~Byte_fixture()
{
}

void Byte_fixture::SetUp()
{
}

void Byte_fixture::TearDown()
{
}

TEST_F(Byte_fixture, Size_of_Byte_8_is_1)
{
	EXPECT_EQ(1, sizeof(Byte_8));
}

TEST_F(Byte_fixture, String_constructor)
{
	Byte_8 byte;
	const Byte_8 emptyByte;
	std::string bitsAsString;

	bitsAsString = "0001"; //too short
	byte = Byte_8(bitsAsString);
	EXPECT_EQ("00000000", byte.getAsString());

	bitsAsString = "000000001"; //too long
	byte = Byte_8(bitsAsString);
	EXPECT_EQ("00000000", byte.getAsString());

	bitsAsString = "00030001"; //wrong char
	byte = Byte_8(bitsAsString);
	EXPECT_EQ("00000000", byte.getAsString());

	bitsAsString = "00000001"; //ok
	byte = Byte_8(bitsAsString);
	EXPECT_EQ("00000001", byte.getAsString());
}

TEST_F(Byte_fixture, char_array_constructor)
{
	Byte_8 byte;
	const Byte_8 emptyByte;

	byte = Byte_8("0001"); //too short
	EXPECT_EQ(emptyByte, byte);

	byte = Byte_8("000000001"); //too long
	EXPECT_EQ(emptyByte, byte);

	byte = Byte_8("00030001"); //wrong char
	EXPECT_EQ(emptyByte, byte);

	byte = Byte_8("00000001"); //ok
	EXPECT_EQ(true, byte.getBit(BIT_0));
}

TEST_F(Byte_fixture, char_constructor)
{
	const char character = 'h'; //01101000
	Byte_8 byte(character);

	EXPECT_EQ("01101000", byte.getAsString());
}

TEST_F(Byte_fixture, Print_empty_byte)
{
	Byte_8 emptyByte;
	EXPECT_EQ("00000000", emptyByte.getAsString());
}

TEST_F(Byte_fixture, Set_single_bits_and_print)
{
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	EXPECT_EQ("00000001", byte.getAsString());

	byte.setBit(BIT_1, true);
	EXPECT_EQ("00000011", byte.getAsString());

	byte.setBit(BIT_2, true);
	EXPECT_EQ("00000111", byte.getAsString());

	byte.setBit(BIT_3, true);
	EXPECT_EQ("00001111", byte.getAsString());

	byte.setBit(BIT_4, true);
	EXPECT_EQ("00011111", byte.getAsString());

	byte.setBit(BIT_5, true);
	EXPECT_EQ("00111111", byte.getAsString());

	byte.setBit(BIT_6, true);
	EXPECT_EQ("01111111", byte.getAsString());

	byte.setBit(BIT_7, true);
	EXPECT_EQ("11111111", byte.getAsString());
}

TEST_F(Byte_fixture, Get_single_bit)
{
	Byte_8 byte;

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
		EXPECT_EQ(false, byte.getBit(i));

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
		byte.setBit(i, true);

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
		EXPECT_EQ(true, byte.getBit(i));
}

TEST_F(Byte_fixture, Clear_byte)
{
	Byte_8 byte;

	byte.setBit(BIT_0, true);

	//Trigger and Check
	byte.clear();
	EXPECT_EQ("00000000", byte.getAsString());
}

TEST_F(Byte_fixture, Get_wrong_index)
{	
	Byte_8 byte;
		
	EXPECT_FALSE(byte.getBit(8)); //too high
	EXPECT_FALSE(byte.getBit(-1)); //too low
}

TEST_F(Byte_fixture, Set_wrong_index)
{
	Byte_8 byte;

	EXPECT_FALSE(byte.setBit(8, true)); //too high
	EXPECT_FALSE(byte.setBit(-1, true)); //too low
}

TEST_F(Byte_fixture, Get_as_bool_vector)
{	
	Byte_8 byte;
	auto boolVector = byte.getAsBoolVector();

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
		EXPECT_EQ(false, boolVector[i]);

	byte.setBit(BIT_2, true);
	boolVector = byte.getAsBoolVector();
	
	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
	{ 
		if (i != BIT_2)
			EXPECT_EQ(false, boolVector[i]);
		else
			EXPECT_EQ(true, boolVector[i]);
	}
}

TEST_F(Byte_fixture, Get_as_int_vector)
{
	Byte_8 byte;
	auto intVector = byte.getAsIntVector();

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
		EXPECT_EQ(0, intVector[i]);

	byte.setBit(BIT_2, true);
	intVector = byte.getAsIntVector();

	for (int i = 0; i < Byte_8::BITS_IN_BYTE; i++)
	{
		if (i != BIT_2)
			EXPECT_EQ(0, intVector[i]);
		else
			EXPECT_EQ(1, intVector[i]);
	}
}

TEST_F(Byte_fixture, Check_if_empty)
{
	Byte_8 byte;

	EXPECT_TRUE(byte.isEmpty());

	byte.setBit(BIT_1, true);
	EXPECT_FALSE(byte.isEmpty());
}

TEST_F(Byte_fixture, Equal_to_operator_for_Byte)
{
	Byte_8 byteFirst;
	Byte_8 byteSecond;

	EXPECT_TRUE(byteFirst == byteSecond);

	byteFirst.setBit(BIT_0, true);
	EXPECT_FALSE(byteFirst == byteSecond);
}

TEST_F(Byte_fixture, Not_equal_to_operator_for_Byte)
{
	Byte_8 byteFirst;
	Byte_8 byteSecond;

	EXPECT_FALSE(byteFirst != byteSecond);

	byteFirst.setBit(BIT_0, true);
	EXPECT_TRUE(byteFirst != byteSecond);
}

TEST_F(Byte_fixture, Equal_to_operator_for_string)
{
	Byte_8 byte;
	std::string byteAsString;

	byteAsString = "000001";
	EXPECT_FALSE(byte == byteAsString); //too short

	byteAsString = "000000001";
	EXPECT_FALSE(byte == byteAsString); //too long

	byteAsString = "000h0001";
	EXPECT_FALSE(byte == byteAsString); //wrong chars

	byteAsString = "00000010";
	EXPECT_FALSE(byte == byteAsString); //not equal

	byteAsString = "00000000";
	EXPECT_TRUE(byte == byteAsString); //equal

	byte.setBit(BIT_0, true);

	byteAsString = "000001";
	EXPECT_FALSE(byte == byteAsString); //too short

	byteAsString = "000000001";
	EXPECT_FALSE(byte == byteAsString); //too long

	byteAsString = "000h0001";
	EXPECT_FALSE(byte == byteAsString); //wrong chars

	byteAsString = "00000010";
	EXPECT_FALSE(byte == byteAsString); //not equal

	byteAsString = "00000001";
	EXPECT_TRUE(byte == byteAsString); //equal
}

TEST_F(Byte_fixture, Not_equal_to_operator_for_string)
{
	Byte_8 byte;
	std::string byteAsString;

	byteAsString = "000001";
	EXPECT_TRUE(byte != byteAsString); //too short

	byteAsString = "000000001";
	EXPECT_TRUE(byte != byteAsString); //too long

	byteAsString = "000h0001";
	EXPECT_TRUE(byte != byteAsString); //wrong chars

	byteAsString = "00000010";
	EXPECT_TRUE(byte != byteAsString); //not equal

	byteAsString = "00000000";
	EXPECT_FALSE(byte != byteAsString); //equal

	byte.setBit(BIT_0, true);

	byteAsString = "000001";
	EXPECT_TRUE(byte != byteAsString); //too short

	byteAsString = "000000001";
	EXPECT_TRUE(byte != byteAsString); //too long

	byteAsString = "000h0001";
	EXPECT_TRUE(byte != byteAsString); //wrong chars

	byteAsString = "00000010";
	EXPECT_TRUE(byte != byteAsString); //not equal

	byteAsString = "00000001";
	EXPECT_FALSE(byte != byteAsString); //equal
}

TEST_F(Byte_fixture, Equal_to_operator_for_char_array)
{
	Byte_8 byte;

	EXPECT_FALSE(byte == "000001"); //too short
	EXPECT_FALSE(byte == "000000001"); //too long
	EXPECT_FALSE(byte == "000h0001"); //wrong chars
	EXPECT_FALSE(byte == "00000010"); //not equal
	EXPECT_TRUE(byte == "00000000"); //equal

	byte.setBit(BIT_0, true);

	EXPECT_FALSE(byte == "000001"); //too short
	EXPECT_FALSE(byte == "000000001"); //too long
	EXPECT_FALSE(byte == "000h0001"); //wrong chars
	EXPECT_FALSE(byte == "00000010"); //not equal
	EXPECT_TRUE(byte == "00000001"); //equal
}

TEST_F(Byte_fixture, Not_equal_to_operator_for_char_array)
{
	Byte_8 byte;

	EXPECT_TRUE(byte != "000001"); //too short
	EXPECT_TRUE(byte != "001000001"); //too long
	EXPECT_TRUE(byte != "000h0001"); //wrong chars
	EXPECT_TRUE(byte != "00000010"); //not equal
	EXPECT_FALSE(byte != "00000000"); //equal

	byte.setBit(BIT_0, true);

	EXPECT_TRUE(byte != "000001"); //too short
	EXPECT_TRUE(byte != "000000001"); //too long
	EXPECT_TRUE(byte != "000h0001"); //wrong chars
	EXPECT_TRUE(byte != "00000010"); //not equal
	EXPECT_FALSE(byte != "00000001"); //equal
}

TEST_F(Byte_fixture, Use_setBit_with_default_value)
{
	Byte_8 byte;
	byte.setBit(0);
	byte.setBit(7);

	EXPECT_TRUE("10000001" == byte);
}