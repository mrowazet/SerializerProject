#include "Serializer_fixture.h"
#include "Byte.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace ::testing;
using namespace ::srl;

Serializer_fixture::Serializer_fixture()
{

}

Serializer_fixture::~Serializer_fixture()
{

}

void Serializer_fixture::SetUp()
{

}

void Serializer_fixture::TearDown()
{

}

TEST_F(Serializer_fixture, Constructor_ByteArray)
{
	ByteArray byteArray;
	Byte_8 byte("11001101");

	byteArray << byte << byte << byte << byte;
	
	Serializer serializer(byteArray);

	EXPECT_EQ(byteArray.size(), serializer.getData().size());
	EXPECT_EQ(byteArray, serializer.getData());
	EXPECT_EQ(byteArray.size(), serializer.getWriteIndex());

	serializer << Byte_8();
	EXPECT_EQ("1100110111001101110011011100110100000000", serializer.getData().getAsString());
}

TEST_F(Serializer_fixture, Add_Byte)
{
	Serializer serializer;
	Byte_8 byte;

	//Trigger and Check
	serializer << byte;
	EXPECT_EQ(ONE_BYTE, serializer.getData().size());
}

TEST_F(Serializer_fixture, Add_Byte_cascaded)
{
	Serializer serializer;
	Byte_8 byte;

	//Trigger and Check
	serializer << byte << byte << byte;
	EXPECT_EQ(THREE_BYTES, serializer.getData().size());
}

TEST_F(Serializer_fixture, Get_Byte_serializer_empty_value_should_not_changed)
{
	Serializer serializer;
	
	Byte_8 byte;
	byte.setBit(BIT_0, true);

	const Byte_8 ByteToMatch(byte);

	testing::internal::CaptureStdout();
	serializer >> byte;
	auto output = testing::internal::GetCapturedStdout();

	EXPECT_EQ("Empty! Not possible to read! \n", output);
	EXPECT_EQ(ByteToMatch, byte);
}

TEST_F(Serializer_fixture, Get_Byte_serializer_not_empty)
{
	Serializer serializer;
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	serializer << byte;

	//Trigger and Check
	serializer >> byte;

	EXPECT_EQ("00000001", byte.getAsString());
}

TEST_F(Serializer_fixture, Get_Byte_serializer_not_empty_cascaded)
{
	Serializer serializer;
	Byte_8 byte;

	byte.setBit(BIT_0, true); 
	serializer << byte;

	byte.setBit(BIT_1, true);
	serializer << byte;

	//Trigger and Check
	Byte_8 first, second;
	serializer >> first >> second;

	EXPECT_EQ("00000001", first.getAsString());
	EXPECT_EQ("00000011", second.getAsString());
}

TEST_F(Serializer_fixture, Stored_Bytes_value)
{
	Serializer serializer;
	Byte_8 byte;

	EXPECT_EQ(ZERO_BYTES, serializer.size());

	serializer << byte;
	EXPECT_EQ(ONE_BYTE, serializer.size());

	serializer << byte;
	EXPECT_EQ(TWO_BYTES, serializer.size());
}

TEST_F(Serializer_fixture, IsEmpty)
{
	Serializer serializer;
	Byte_8 byte;

	EXPECT_TRUE(serializer.isEmpty());

	serializer << byte;
	EXPECT_FALSE(serializer.isEmpty());

	serializer.clear();
	EXPECT_TRUE(serializer.isEmpty());
}

TEST_F(Serializer_fixture, boolOperator)
{
	Serializer serializer;
	Byte_8 byte;

	EXPECT_TRUE(serializer);

	serializer << byte;
	EXPECT_FALSE(serializer);

	serializer.clear();
	EXPECT_TRUE(serializer);
}

TEST_F(Serializer_fixture, Current_write_index_value)
{
	Serializer serializer;
	Byte_8 byte;

	EXPECT_EQ(ZERO_BYTES, serializer.getData().size());
	EXPECT_EQ(BYTE_0, serializer.getWriteIndex());

	serializer << byte << byte << byte;

	EXPECT_EQ(BYTE_3, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Current_read_index_value)
{
	Serializer serializer;
	Byte_8 byte;

	EXPECT_EQ(BYTE_0, serializer.getReadIndex());

	serializer << byte << byte << byte;
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());

	serializer >> byte >> byte >> byte;
	EXPECT_EQ(BYTE_3, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Set_write_index)
{
	Serializer serializer;
	const int NegativeByteIndex = -1;

	EXPECT_EQ(false, serializer.setWriteIndex(NegativeByteIndex));
	EXPECT_EQ(true, serializer.setWriteIndex(BYTE_0));
	EXPECT_EQ(false, serializer.setWriteIndex(BYTE_1));

	Byte_8 byte;
	serializer << byte << byte;

	EXPECT_EQ(false, serializer.setWriteIndex(NegativeByteIndex));
	EXPECT_EQ(true, serializer.setWriteIndex(BYTE_0));
	EXPECT_EQ(true, serializer.setWriteIndex(BYTE_2));

	//Check if ReadIndex did not change
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Set_read_index)
{
	Serializer serializer;
	const int NegativeByteIndex = -1;

	EXPECT_EQ(false, serializer.setReadIndex(NegativeByteIndex));
	EXPECT_EQ(false, serializer.setReadIndex(BYTE_0));
	EXPECT_EQ(false, serializer.setReadIndex(BYTE_1));

	Byte_8 byte;
	serializer << byte << byte;

	EXPECT_EQ(false, serializer.setReadIndex(NegativeByteIndex));
	EXPECT_EQ(true, serializer.setReadIndex(BYTE_0));
	EXPECT_EQ(false, serializer.setReadIndex(BYTE_2));
}

TEST_F(Serializer_fixture, Read_from_selected_index)
{
	Serializer serializer;

	Byte_8 byte = Byte_8("00000001");
	serializer << byte;

	byte = Byte_8("00000011");
	serializer << byte;

	byte = Byte_8("00000111");
	serializer << byte;

	EXPECT_EQ("000000010000001100000111", serializer.getData().getAsString());

	//Check
	serializer >> byte;

	EXPECT_EQ(BYTE_1, serializer.getReadIndex());
	EXPECT_EQ("00000001", byte.getAsString());

	serializer >> byte;

	EXPECT_EQ(BYTE_2, serializer.getReadIndex());
	EXPECT_EQ("00000011", byte.getAsString());

	serializer.setReadIndex(BYTE_0);
	serializer >> byte;

	EXPECT_EQ(BYTE_1, serializer.getReadIndex());
	EXPECT_EQ("00000001", byte.getAsString());
}

TEST_F(Serializer_fixture, Write_to_selected_index)
{
	Serializer serializer;

	Byte_8 byte = Byte_8("00000001");
	serializer << byte;

	byte = Byte_8("00000011");
	serializer << byte;

	byte = Byte_8("00000111");
	serializer << byte;

	EXPECT_EQ("000000010000001100000111", serializer.getData().getAsString());

	//Check
	byte = Byte_8("11111111");

	serializer.setWriteIndex(BYTE_0);
	serializer << byte;

	serializer.setWriteIndex(BYTE_2);
	serializer << byte;

	EXPECT_EQ("111111110000001111111111", serializer.getData().getAsString());
}

TEST_F(Serializer_fixture, Clear)
{
	Serializer serializer;
	Byte_8 byte;
	
	serializer << byte << byte << byte;
	serializer >> byte;

	EXPECT_EQ(THREE_BYTES, serializer.getData().size());
	EXPECT_EQ(BYTE_1, serializer.getReadIndex());
	EXPECT_EQ(BYTE_3, serializer.getWriteIndex());

	//Trigger and Check
	serializer.clear();

	EXPECT_EQ(ZERO_BYTES, serializer.getData().size());
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
	EXPECT_EQ(BYTE_0, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Add_int_to_empty) //check!
{
	Serializer serializer;
	int testValue = 252117761; //00000001 00000011 00000111 00001111 - little endian

	testing::internal::CaptureStdout();
	serializer << testValue;
	auto output = testing::internal::GetCapturedStdout();

	ASSERT_EQ(FOUR_BYTES, serializer.size());
	EXPECT_EQ("", output);

	auto data = serializer.getData();
	EXPECT_EQ("00000001", data[0].getAsString());
	EXPECT_EQ("00000011", data[1].getAsString());
	EXPECT_EQ("00000111", data[2].getAsString());
	EXPECT_EQ("00001111", data[3].getAsString());

	EXPECT_EQ(FOUR_BYTES, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Add_int_to_not_empty)
{
	Serializer serializer;
	Byte_8 byte("11111111");
	int testValue = 252117761; //00000001 00000011 00000111 00001111 - little endian

	serializer << byte;
	serializer << testValue;

	EXPECT_EQ(FIVE_BYTES, serializer.size());
	EXPECT_EQ("1111111100000001000000110000011100001111", serializer.getData().getAsString());
	EXPECT_EQ(FIVE_BYTES, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Get_int_from_empty)
{
	Serializer serializer;
	int value = 1;
	int orginalValue = value;

	testing::internal::CaptureStdout();
	serializer >> value;
	auto output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(orginalValue, value);
	EXPECT_EQ("Empty! Not possible to read! \n", output);
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Get_int_from_not_empty)
{
	Serializer serializer;
	int testValue = 543;
	int orginalValue = testValue;

	serializer << testValue;
	testValue = 0;

	serializer >> testValue;

	EXPECT_EQ(orginalValue, testValue);
	EXPECT_EQ(FOUR_BYTES, serializer.getReadIndex());
	EXPECT_EQ(FOUR_BYTES, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Get_int_from_not_empty_but_too_short)
{
	Serializer serializer;
	Byte_8 byte("00000011");
	int testValue = 1;
	int orginalValue = testValue;

	serializer << byte;

	testing::internal::CaptureStdout();
	serializer >> testValue;
	auto output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(orginalValue, testValue);
	EXPECT_EQ("Stored data too short! Not possible to read! \n", output);
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
	EXPECT_EQ(ONE_BYTE, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Add_int_when_write_index_is_set_and_one_byte_more_needed)
{
	Serializer serializer;
	Byte_8 byte("11111111");
	int testValue = 252117761; //00000001 00000011 00000111 00001111 - little endian

	serializer << byte << byte << byte << byte;
	EXPECT_EQ(FOUR_BYTES, serializer.size());
	
	EXPECT_TRUE(serializer.setWriteIndex(1));

	serializer << testValue;
	EXPECT_EQ(FIVE_BYTES, serializer.size());
	EXPECT_EQ("1111111100000001000000110000011100001111", serializer.getData().getAsString());

	EXPECT_EQ(BYTE_5, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Set_write_index_after_last_element_and_add_byte)
{
	ByteArray emptyByteArray(FOUR_BYTES);
	Serializer serializer(emptyByteArray);
	Byte_8 testByte("00111100");
	
	EXPECT_EQ("00000000000000000000000000000000", serializer.getData().getAsString());

	serializer.setWriteIndex(BYTE_0);
	serializer << testByte;
	EXPECT_EQ("00111100000000000000000000000000", serializer.getData().getAsString());

	const int Index = serializer.size();
	serializer.setWriteIndex(Index);

	serializer << testByte;

	EXPECT_EQ(FIVE_BYTES, serializer.size());
	EXPECT_EQ("0011110000000000000000000000000000111100", serializer.getData().getAsString());
}

TEST_F(Serializer_fixture, Get_int_when_read_index_is_set_to_specific_position)
{
	Serializer serializer;
	
	serializer << Byte_8("00111111") << Byte_8("00011111") << Byte_8("00001111");
	serializer << Byte_8("00000111") << Byte_8("00000011") << Byte_8("00000001");
	EXPECT_EQ("001111110001111100001111000001110000001100000001", serializer.getData().getAsString());

	serializer.setReadIndex(BYTE_2);

	int result = 0;
	serializer >> result; 
	
	//16975631 00001111000001110000001100000001 - little endian
	const int ExpectedResult = 16975631;

	EXPECT_EQ(ExpectedResult, result);
	EXPECT_EQ(SIX_BYTES, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Get_int_when_read_index_is_set_before_last)
{
	Serializer serializer;
	serializer << Byte_8("00111111") << Byte_8("00011111") << Byte_8("00001111") << Byte_8("00000111");

	serializer.setReadIndex(BYTE_2);
	int result = 1;
	int orginalValue = result;

	testing::internal::CaptureStdout();
	serializer >> result;
	auto output = testing::internal::GetCapturedStdout();

	EXPECT_EQ(orginalValue, result);
	EXPECT_EQ("Stored data too short! Not possible to read! \n", output);
	EXPECT_EQ(BYTE_2, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Add_int_in_the_middle)
{
	Serializer serializer;
	int testValue = 0;

	serializer << Byte_8("00111111") << Byte_8("00011111") << Byte_8("00001111");
	serializer << Byte_8("00000111") << Byte_8("00000011") << Byte_8("00000001");

	serializer.setWriteIndex(BYTE_1);
	serializer << testValue;

	EXPECT_EQ(SIX_BYTES, serializer.size());
	EXPECT_EQ("001111110000000000000000000000000000000000000001", serializer.getData().getAsString());
	EXPECT_EQ(BYTE_5, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Get_int_from_the_middle)
{
	Serializer serializer;
	int testValue = 0;

	serializer << Byte_8("00111111") << Byte_8("00011111") << Byte_8("00001111");
	serializer << Byte_8("00000111") << Byte_8("00000011") << Byte_8("00000001");

	serializer.setReadIndex(1);
	serializer >> testValue;

	//50794271 - 00000011 00000111 00001111 00011111
	const int ExpectedResult = 50794271;

	EXPECT_EQ(BYTE_5, serializer.getReadIndex());
	EXPECT_EQ(ExpectedResult, testValue);
}

TEST_F(Serializer_fixture, Add_get_unsigned_int)
{
	Serializer serializer;
	const unsigned int testValue = 43287;

	serializer << testValue;

	unsigned int result;
	serializer >> result;

	EXPECT_EQ(testValue, result);
}

TEST_F(Serializer_fixture, Add_get_char)
{
	Serializer serializer;
	const char testValue = 'a';

	serializer << testValue;

	char result;
	serializer >> result;

	EXPECT_EQ(testValue, result);
	EXPECT_EQ(serializer.getData().getAsString(), "01100001");
}

TEST_F(Serializer_fixture, Add_get_short)
{
	Serializer serializer;
	const short testValue = 563;

	serializer << testValue;

	short result;
	serializer >> result;

	EXPECT_EQ(testValue, result);
	EXPECT_EQ(serializer.getData().getAsString(), "0011001100000010");
}

TEST_F(Serializer_fixture, Add_get_float)
{
	Serializer serializer;
	const float testValue = 5683.432;

	serializer << testValue;

	float result;
	serializer >> result;

	EXPECT_FLOAT_EQ(testValue, result);
}

TEST_F(Serializer_fixture, Add_get_double)
{
	Serializer serializer;
	const double testValue = 43536.3;

	serializer << testValue;

	double result;
	serializer >> result;

	EXPECT_DOUBLE_EQ(testValue, result);
}

TEST_F(Serializer_fixture, Add_get_long)
{
	Serializer serializer;
	const long testValue = 333472.9384;

	serializer << testValue;

	long result;
	serializer >> result;

	EXPECT_EQ(testValue, result);
}

TEST_F(Serializer_fixture, Add_get_long_long)
{
	Serializer serializer;
	const long long testValue = 563;

	serializer << testValue;

	long long result;
	serializer >> result;

	EXPECT_EQ(testValue, result);
}

TEST_F(Serializer_fixture, Add_ByteArray_to_empty)
{
	Serializer serializer;

	ByteArray byteArray;
	byteArray << "00011000" << "00011000" << "00011000" << "00011000";

	serializer << byteArray;
	
	EXPECT_EQ(byteArray.size(), serializer.size());
	EXPECT_EQ(BYTE_4, serializer.getWriteIndex());
	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
	EXPECT_EQ("00011000000110000001100000011000", serializer.getData().getAsString());
}

TEST_F(Serializer_fixture, Add_ByteArray_to_not_empty)
{
	Serializer serializer;
	const int TestInt = 6;

	ByteArray byteArray;
	byteArray << "00011000" << "00011000" << "00011000" << "00011000";

	serializer << TestInt << byteArray;

	const int ExpectedSize = sizeof(TestInt) + byteArray.size();
	EXPECT_EQ(ExpectedSize, serializer.size());
	EXPECT_EQ("0000011000000000000000000000000000011000000110000001100000011000", serializer.getData().getAsString());
}

TEST_F(Serializer_fixture, Check_write_index_after_write_ByteArray)
{
	Serializer serializer;
	const int TestInt = 6;

	ByteArray byteArray(BYTE_5);

	serializer << TestInt << byteArray;

	const int ExpectedWriteIndex = sizeof(TestInt) + byteArray.size();
	EXPECT_EQ(ExpectedWriteIndex, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Add_string_to_empty_check_size)
{
	const std::string TestString = "String to test Serializer operators";
	Serializer serializer;

	serializer << TestString;

	const int ExpectedSize = sizeof(int) + TestString.length();

	EXPECT_EQ(ExpectedSize, serializer.size());
}

TEST_F(Serializer_fixture, Add_string_to_not_empty_check_size)
{
	const std::string TestString = "String to test Serializer operators";
	const int TestValue = 76;
	Serializer serializer;

	serializer << TestValue;
	serializer << TestString;

	const int ExpectedSize = sizeof(TestValue) + sizeof(int) + TestString.length();

	EXPECT_EQ(ExpectedSize, serializer.size());	
}

TEST_F(Serializer_fixture, Get_string_when_only_string_stored)
{
	const std::string TestString = "String to test Serializer operators";
	Serializer serializer;

	serializer << TestString;

	std::string result;
	serializer >> result;

	EXPECT_EQ(TestString, result);
}

TEST_F(Serializer_fixture, Get_string_when_not_only_string_stored)
{
	const std::string TestString = "String to test Serializer operators";
	const int TestValue = 76;
	Serializer serializer;

	serializer << TestValue;
	serializer << TestString;

	int storedInt;
	serializer >> storedInt;

	std::string result;
	serializer >> result;

	EXPECT_EQ(TestString, result);
}

TEST_F(Serializer_fixture, Get_mixed_int_string_int)
{
	const std::string TestString = "String to test Serializer operators";
	const int TestValue = 76;
	Serializer serializer;

	serializer << TestValue << TestString << TestValue;

	int intResult_first, intResultSecond;
	std::string stringResult;

	serializer >> intResult_first;
	serializer >> stringResult;
	serializer >> intResultSecond;

	EXPECT_EQ(TestValue, intResult_first);
	EXPECT_EQ(TestString, stringResult);
	EXPECT_EQ(TestValue, intResultSecond);
}

TEST_F(Serializer_fixture, Check_read_index_after_read_string)
{
	const std::string TestString = "String to test Serializer operators";
	const int TestValue = 76;
	Serializer serializer;

	serializer << TestString << TestValue;

	std::string result;
	serializer >> result;

	const int ExpectedIndex = sizeof(int) + TestString.size();
	EXPECT_EQ(ExpectedIndex, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Add_get_two_strings)
{
	const std::string TestString_first = "First string to test";
	const std::string TestString_second = "Second string under test";
	Serializer serializer;

	serializer << TestString_first << TestString_second;

	const int ExpectedSize = sizeof(int) + TestString_first.size() + sizeof(int) + TestString_second.size();
	EXPECT_EQ(ExpectedSize, serializer.size());

	std::string result_first, result_second;
	serializer >> result_first;
	serializer >> result_second;

	EXPECT_EQ(TestString_first, result_first);
	EXPECT_EQ(TestString_second, result_second);
}

TEST_F(Serializer_fixture, Check_write_index_after_write_string)
{
	const std::string TestString = "String to test Serializer operators";
	const int TestValue = 76;
	Serializer serializer;

	serializer << TestValue << TestString;

	const int ExpectedWriteIndex = sizeof(TestValue) + sizeof(int) + TestString.size();
	EXPECT_EQ(ExpectedWriteIndex, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Read_string_from_empty_serializer)
{
	Serializer serializer;

	std::string result;
	serializer >> result;

	EXPECT_TRUE(result.empty());
}

TEST_F(Serializer_fixture, Read_string_incorrectly_when_only_some_value_is_stored_and_indicated_string_is_too_long)
{
	Serializer serializer;
	const int IntValue = 9; //interpreted as string size indicator

	serializer << IntValue << IntValue << IntValue;

	std::string result;
	serializer >> result;

	EXPECT_TRUE(result.empty());
	EXPECT_EQ("", result);
}

TEST_F(Serializer_fixture, Read_index_is_on_correct_position_after_incorect_read_of_string_and_indicated_string_is_too_long)
{
	Serializer serializer;
	const int IntValue = 9; //interpreted as string size indicator

	serializer << IntValue << IntValue << IntValue;

	std::string result;
	serializer >> result;

	EXPECT_EQ(BYTE_0, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Write_read_empty_string)
{
	Serializer serializer;
	const std::string EmptyString = "";

	serializer << EmptyString;

	std::string result = "something";
	serializer >> result;

	EXPECT_EQ(EmptyString, result);
}

TEST_F(Serializer_fixture, Method_getAsString_called_when_serialize_used)
{
	const std::string TestString = "Default_mock_test_string";
	StrictMock<SerializableMock> serializableMock;
	Serializer serializer;

	EXPECT_CALL(serializableMock, getAsString()).WillOnce(Return(TestString));
	serializer << serializableMock.asSerializable();
}

TEST_F(Serializer_fixture, Correct_string_stored_when_asSerializble_used)
{
	const std::string TestString = "Default_mock_test_string";
	SerializableMock serializableMock;
	Serializer serializer;

	EXPECT_CALL(serializableMock, getAsString()).WillOnce(Return(TestString));
	serializer << serializableMock.asSerializable();

	std::string storedData;
	serializer >> storedData;

	EXPECT_EQ(TestString, storedData);
}

TEST_F(Serializer_fixture, InitFromString_called_when_read_to_asSerializable)
{
	const std::string TestString("TestData");
	StrictMock<SerializableMock> serializableMock;

	Serializer serializer;
	serializer << TestString;

	EXPECT_CALL(serializableMock, initFromString(_)).Times(1);
	serializer >> serializableMock.asSerializable();
}

TEST_F(Serializer_fixture, InitFromString_called_with_expected_string_when_read_to_asSerializable)
{
	const std::string TestString("TestData");
	StrictMock<SerializableMock> serializableMock;

	Serializer serializer;
	serializer << TestString;

	auto matcher = Eq(TestString);
	EXPECT_CALL(serializableMock, initFromString(matcher)).Times(1);

	serializer >> serializableMock.asSerializable();
}

TEST_F(Serializer_fixture, InitFromString_not_called_when_read_to_asSerializable_but_stored_string_is_incorrect)
{
	const int TestInt = 9;
	const std::string TestString("");
	StrictMock<SerializableMock> serializableMock;
	EXPECT_CALL(serializableMock, initFromString(_)).Times(0);

	Serializer serializer;
	serializer << TestInt;

	serializer >> serializableMock.asSerializable();

	serializer.clear();
	serializer << TestString;

	serializer >> serializableMock.asSerializable();
}

TEST_F(Serializer_fixture, Add_c_str)
{
	const std::string testString = "Test_c_string_to_store";
	Serializer serializer;

	serializer << testString.c_str();

	std::string result;
	serializer >> result;

	EXPECT_EQ(testString, result);
}

TEST_F(Serializer_fixture, Add_empty_c_str)
{
	const std::string testString = "";
	Serializer serializer;

	serializer << testString.c_str();

	std::string result = "something";
	serializer >> result;

	EXPECT_EQ(testString, result);
}

TEST_F(Serializer_fixture, Add_c_str_moves_writeIndex)
{
	const std::string testString = "Some_test_c_str";
	Serializer serializer;

	const int ExpectedWriteIndex = sizeof(int) + testString.size();

	serializer << testString.c_str();

	EXPECT_EQ(ExpectedWriteIndex, serializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Constructor_with_move_semantic)
{
	Serializer firstSerializer;	
	firstSerializer << '1' << '5';

	char test;
	firstSerializer >> test;

	Serializer secondSerializer(std::move(firstSerializer));

	EXPECT_EQ(BYTE_0, firstSerializer.getReadIndex());
	EXPECT_EQ(BYTE_0, firstSerializer.getWriteIndex());	
	EXPECT_EQ(ZERO_BYTES, firstSerializer.size());
	EXPECT_TRUE(firstSerializer.isEmpty());

	EXPECT_EQ(TWO_BYTES, secondSerializer.size());
	EXPECT_EQ(BYTE_1, secondSerializer.getReadIndex());
	EXPECT_EQ(BYTE_2, secondSerializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Move_assignment_operator)
{
	Serializer firstSerializer;
	Serializer secondSerializer;

	firstSerializer << 5 << 2;
	firstSerializer.setReadIndex(BYTE_2);

	secondSerializer = std::move(firstSerializer);

	EXPECT_EQ(BYTE_0, firstSerializer.getReadIndex());
	EXPECT_EQ(BYTE_0, firstSerializer.getWriteIndex());
	EXPECT_EQ(ZERO_BYTES, firstSerializer.size());
	EXPECT_TRUE(firstSerializer.isEmpty());

	EXPECT_EQ(EIGHT_BYTES, secondSerializer.size());
	EXPECT_EQ(BYTE_2, secondSerializer.getReadIndex());
	EXPECT_EQ(BYTE_8, secondSerializer.getWriteIndex());
}

TEST_F(Serializer_fixture, Read_from_const_serializer_using_template)
{
	const int TestValue = 5;
	Serializer storage;
	storage << TestValue << 7 << 2 << 27;

	const Serializer serializer(std::move(storage));

	int result = 0;
	serializer >> result;

	EXPECT_EQ(TestValue, result);
	EXPECT_EQ(sizeof(int), serializer.getReadIndex());
}

TEST_F(Serializer_fixture, Read_string_from_const_serializer)
{
	const std::string TestString = "This is string for test";
	Serializer storage;
	storage << TestString;

	const Serializer serializer(std::move(storage));

	std::string result;
	serializer >> result;

	EXPECT_EQ(TestString, result);
}

TEST_F(Serializer_fixture, Read_ISerializable_from_const_serializer)
{
	StrictMock<SerializableMock> serializableMock;
	const std::string SerializedData = "This is string for test";
	EXPECT_CALL(serializableMock, getAsString()).WillOnce(Return(SerializedData));

	Serializer storage;
	storage << serializableMock.asSerializable();

	const Serializer serializer(std::move(storage));

	auto matcher = Eq(SerializedData);
	EXPECT_CALL(serializableMock, initFromString(matcher)).WillOnce(Return(true));

	serializer >> serializableMock.asSerializable();
}

TEST_F(Serializer_fixture, Use_const_ISerializable)
{
	const std::string TestString = "This is string for test";
	const SerializableTestClass TestObject(TestString);

	Serializer serializer;
	serializer << TestObject.asSerializable();

	std::string result;
	serializer >> result;

	EXPECT_EQ(TestString, result);
}

TEST_F(Serializer_fixture, Set_read_index_on_const_serializer)
{
	Serializer storage;
	storage << 5;

	const Serializer serializer(std::move(storage));
	
	EXPECT_TRUE(serializer.setReadIndex(BYTE_2));
	EXPECT_EQ(BYTE_2, serializer.getReadIndex());
}

TEST_F(Serializer_fixture, At_returns_correct_element_reference)
{
	const int TestValue = 7;
	const int ExpectedValue = 15;
	Serializer serializer;

	serializer << TestValue;

	Byte_8 & element = serializer.at(BYTE_0);
	element = Byte_8("00001111"); //ExpectedValue = 15

	int result;
	serializer >> result;

	EXPECT_EQ(ExpectedValue, result);
}

TEST_F(Serializer_fixture, Const_At_returns_correct_element_reference)
{
	const int TestValue = 7;
	Serializer storage;

	storage << TestValue;

	const Serializer serializer(std::move(storage));
	const Byte_8 & element = serializer.at(BYTE_0);

	EXPECT_EQ("00000111", element.getAsString());
}

TEST_F(Serializer_fixture, At_throws_exception_if_out_of_range)
{
	Serializer serializer;
	EXPECT_THROW(serializer.at(BYTE_4), std::out_of_range);

	const Serializer serializer_const;
	EXPECT_THROW(serializer_const.at(BYTE_4), std::out_of_range);
}




