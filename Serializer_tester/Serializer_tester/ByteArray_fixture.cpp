#include "ByteArray_fixture.h"
#include <iostream>

using namespace ::srl;

ByteArray_fixture::ByteArray_fixture()
{
}

ByteArray_fixture::~ByteArray_fixture()
{
}

void ByteArray_fixture::SetUp()
{
}

void ByteArray_fixture::TearDown()
{
}

TEST_F(ByteArray_fixture, Default_constructor)
{
	ByteArray byteArray;
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Constructor_with_customized_size)
{
	ByteArray byteArray(ONE_BYTE);
	EXPECT_EQ(ONE_BYTE, byteArray.size());

	byteArray = ByteArray(ZERO_BYTES);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Clear_ByteArray)
{
	ByteArray byteArray(EIGHT_BYTES);

	//Trigger and Check
	byteArray.clear();
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, ByteArray_isZeroed_true)
{
	ByteArray byteArray(EIGHT_BYTES);
	EXPECT_TRUE(byteArray.isZeroed());

	byteArray = ByteArray();
	EXPECT_TRUE(byteArray.isZeroed());

	byteArray = ByteArray(ZERO_BYTES);
	EXPECT_TRUE(byteArray.isZeroed());
}

TEST_F(ByteArray_fixture, ByteArray_isZeroed_false)
{
	ByteArray byteArray(EIGHT_BYTES);

	Byte_8 byte;
	byte.setBit(BIT_0, true);

	//Trigger and Check
	byteArray.setByte(byte, BIT_0);
	EXPECT_FALSE(byteArray.isZeroed());
}

TEST_F(ByteArray_fixture, ByteArray_isEmpty)
{
	ByteArray byteArray(ZERO_BYTES);

	EXPECT_TRUE(byteArray.isEmpty());

	byteArray.resize(ONE_BYTE);
	EXPECT_FALSE(byteArray.isEmpty());
}

TEST_F(ByteArray_fixture, Get_as_string)
{
	ByteArray byteArray(ZERO_BYTES);
	EXPECT_EQ("", byteArray.getAsString());

	byteArray = ByteArray(ONE_BYTE);
	EXPECT_EQ("00000000", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, Get_as_string_byte_order)
{
	ByteArray byteArray(TWO_BYTES);

	Byte_8 byte;
	byte.setBit(BIT_0, true);

	//Trigger and Check
	byteArray.setByte(byte, BYTE_0);
	EXPECT_EQ("0000000100000000", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, Resize_to_the_same_size)
{
	ByteArray byteArray(EIGHT_BYTES);

	byteArray.resize(EIGHT_BYTES);

	//Check
	EXPECT_EQ(EIGHT_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Resize_zero_to_non_zero_value)
{
	ByteArray byteArray(ZERO_BYTES);
	
	testing::internal::CaptureStdout();
	byteArray.resize(EIGHT_BYTES);
	auto output = testing::internal::GetCapturedStdout();

	//Check
	EXPECT_EQ(EIGHT_BYTES, byteArray.size());
	EXPECT_EQ("", output);
}

TEST_F(ByteArray_fixture, Resize_non_zero_to_zero)
{
	ByteArray byteArray(EIGHT_BYTES);
	
	byteArray.resize(ZERO_BYTES);

	//Check
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Resize_new_size_grater_than_current)
{
	ByteArray byteArray(ONE_BYTE);

	testing::internal::CaptureStdout();
	byteArray.resize(EIGHT_BYTES);
	auto output = testing::internal::GetCapturedStdout();

	//Check
	EXPECT_EQ(EIGHT_BYTES, byteArray.size());
	EXPECT_EQ("", output);
}

TEST_F(ByteArray_fixture, Resize_new_size_lower_than_current)
{
	ByteArray byteArray(EIGHT_BYTES);

	byteArray.resize(ONE_BYTE);

	//Check
	EXPECT_EQ(ONE_BYTE, byteArray.size());
}

TEST_F(ByteArray_fixture, SetByte_out_of_bound)
{
	ByteArray byteArray(EIGHT_BYTES);
	Byte_8 byte;

	auto result = byteArray.setByte(byte, EIGHT_BYTES);

	//Check
	EXPECT_FALSE(result);
}

TEST_F(ByteArray_fixture, SetByte_within_index)
{
	ByteArray byteArray(EIGHT_BYTES);
	Byte_8 byte;

	testing::internal::CaptureStdout();
	auto result = byteArray.setByte(byte, EIGHT_BYTES - 1);
	auto output = testing::internal::GetCapturedStdout();

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ("", output);
}

TEST_F(ByteArray_fixture, GetByte_out_of_index_empty_byte_returned)
{
	ByteArray byteArray(EIGHT_BYTES);
	Byte_8 byte("10101010");

	byte = byteArray.getByte(EIGHT_BYTES);

	//Check
	EXPECT_EQ("00000000", byte.getAsString());
}

TEST_F(ByteArray_fixture, GetByte_within_index)
{
	ByteArray byteArray(EIGHT_BYTES);

	testing::internal::CaptureStdout();
	const int LastProperIndex = EIGHT_BYTES - 1;
	auto byte = byteArray.getByte(LastProperIndex);
	auto output = testing::internal::GetCapturedStdout();

	//Check
	EXPECT_EQ("", output);
}

TEST_F(ByteArray_fixture, Set_and_Get_byte_from_the_same_index)
{
	ByteArray byteArray(EIGHT_BYTES);
	Byte_8 byte;
	byte.setBit(BIT_0, true);

	testing::internal::CaptureStdout();

	const int LastProperIndex = EIGHT_BYTES - 1;
	byteArray.setByte(byte, LastProperIndex);
	auto byteReceived = byteArray.getByte(LastProperIndex);

	auto output = testing::internal::GetCapturedStdout();

	//Check
	EXPECT_EQ("", output);
	EXPECT_TRUE(byte == byteReceived);
}

TEST_F(ByteArray_fixture, Equal_to_operator_different_size)
{
	ByteArray byteArrayFirst(ONE_BYTE);
	ByteArray byteArraySecond(EIGHT_BYTES);

	EXPECT_FALSE(byteArrayFirst == byteArraySecond);
}

TEST_F(ByteArray_fixture, Equal_to_operator_same_size)
{
	ByteArray byteArrayFirst(EIGHT_BYTES);
	ByteArray byteArraySecond(EIGHT_BYTES);

	EXPECT_TRUE(byteArrayFirst == byteArraySecond);

	Byte_8 byte;
	byte.setBit(BIT_0, true);
	byteArrayFirst.setByte(byte, BYTE_0);

	EXPECT_FALSE(byteArrayFirst == byteArraySecond);
}

TEST_F(ByteArray_fixture, Not_equal_to_operator_different_size)
{
	ByteArray byteArrayFirst(ONE_BYTE);
	ByteArray byteArraySecond(EIGHT_BYTES);

	EXPECT_TRUE(byteArrayFirst != byteArraySecond);
}

TEST_F(ByteArray_fixture, Not_equal_to_operator_same_size)
{
	ByteArray byteArrayFirst(EIGHT_BYTES);
	ByteArray byteArraySecond(EIGHT_BYTES);

	EXPECT_FALSE(byteArrayFirst != byteArraySecond);

	Byte_8 byte;
	byte.setBit(BIT_0, true);
	byteArrayFirst.setByte(byte, BYTE_0);

	EXPECT_TRUE(byteArrayFirst != byteArraySecond);
}

TEST_F(ByteArray_fixture, Add_operator_empty_to_not_empty)
{
	ByteArray byteArrayFirst(EIGHT_BYTES);
	ByteArray byteArraySecond(ZERO_BYTES);

	//Trigger and Check
	auto result = byteArrayFirst + byteArraySecond;
	EXPECT_EQ(EIGHT_BYTES, result.size());
}

TEST_F(ByteArray_fixture, Add_operator_not_empty_to_empty)
{
	ByteArray byteArrayFirst(ZERO_BYTES);
	ByteArray byteArraySecond(EIGHT_BYTES);

	//Trigger and Check
	auto result = byteArrayFirst + byteArraySecond;
	EXPECT_EQ(EIGHT_BYTES, result.size());
}

TEST_F(ByteArray_fixture, Add_operator_empty_to_empty)
{
	ByteArray byteArrayFirst(ZERO_BYTES);
	ByteArray byteArraySecond(ZERO_BYTES);

	//Trigger and Check
	auto result = byteArrayFirst + byteArraySecond;
	EXPECT_EQ(ZERO_BYTES, result.size());
}

TEST_F(ByteArray_fixture, Add_operator_not_empty_to_not_empty)
{
	ByteArray byteArrayFirst(EIGHT_BYTES);
	ByteArray byteArraySecond(EIGHT_BYTES);

	//Trigger and Check
	auto result = byteArrayFirst + byteArraySecond;

	const int SumOfSize = byteArrayFirst.size() + byteArraySecond.size();
	EXPECT_EQ(SumOfSize, result.size());
}

TEST_F(ByteArray_fixture, Add_operator_cascaded_call)
{
	ByteArray firstByteArray(ONE_BYTE);
	ByteArray secondByteArray(ONE_BYTE);
	ByteArray thirdByteArray(ONE_BYTE);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	firstByteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	secondByteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_2, true);
	thirdByteArray.setByte(byte, BYTE_0);

	//Trigger and Check
	ByteArray result = firstByteArray + secondByteArray + thirdByteArray;
	EXPECT_EQ("000000010000001100000111", result.getAsString());
}

TEST_F(ByteArray_fixture, Add_Byte_8_to_empty_array)
{
	ByteArray byteArray(ZERO_BYTES);
	Byte_8 byte;

	//Trigger and Check
	ByteArray result = byteArray + byte;
	EXPECT_EQ(ONE_BYTE, result.size());
}

TEST_F(ByteArray_fixture, Add_Byte_8_to_not_empty_array)
{
	ByteArray byteArray(ONE_BYTE);
	Byte_8 byte;

	byte.setBit(BIT_0, true);

	//Trigger and Check
	ByteArray result = byteArray + byte;
	EXPECT_EQ(TWO_BYTES, result.size());
	EXPECT_EQ("0000000000000001", result.getAsString());
}

TEST_F(ByteArray_fixture, Add_empty_array_to_Byte_8)
{
	ByteArray byteArray(ZERO_BYTES);
	Byte_8 byte;

	//Trigger and Check
	ByteArray result = byte + byteArray;
	EXPECT_EQ(ONE_BYTE, result.size());
}

TEST_F(ByteArray_fixture, Add_not_empty_array_to_Byte_8)
{
	ByteArray byteArray(ONE_BYTE);
	Byte_8 byte;
	byte.setBit(BIT_0, true);

	//Trigger and Check
	ByteArray result = byte + byteArray;
	EXPECT_EQ(TWO_BYTES, result.size());
	EXPECT_EQ("0000000100000000", result.getAsString());
}

TEST_F(ByteArray_fixture, Copy_constructor)
{
	ByteArray orginal(EIGHT_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	orginal.setByte(byte, BYTE_0);

	//Trigger and Check
	ByteArray copy(orginal);
	EXPECT_TRUE(orginal == copy);
}

TEST_F(ByteArray_fixture, Assignment_operator)
{
	ByteArray first(EIGHT_BYTES);
	ByteArray second(EIGHT_BYTES);

	first = ByteArray(ONE_BYTE);
	EXPECT_EQ(ONE_BYTE, first.size());

	//Trigger and Check
	first = second;
	EXPECT_TRUE(first == second);
	EXPECT_EQ(EIGHT_BYTES, first.size());
	EXPECT_EQ(EIGHT_BYTES, second.size());
}

TEST_F(ByteArray_fixture, Assignment_operator_cascaded_call)
{
	ByteArray first(ONE_BYTE);
	ByteArray second(ONE_BYTE);
	ByteArray third(ZERO_BYTES);

	Byte_8 byte;
	byte.setBit(BIT_0, true);
	first.setByte(byte, BYTE_0);

	//Trigger
	third = second = first;

	//Check
	EXPECT_EQ("00000001", first.getAsString());
	EXPECT_EQ("00000001", second.getAsString());
	EXPECT_EQ("00000001", third.getAsString());
}

TEST_F(ByteArray_fixture, Add_and_assign_operator)
{
	ByteArray first(EIGHT_BYTES);
	ByteArray second(EIGHT_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);

	first.setByte(byte, BYTE_0);
	first.setByte(byte, BYTE_7);

	second.setByte(byte, BYTE_0);
	second.setByte(byte, BYTE_7);

	//Trigger
	const int ExpectedSize = first.size() + second.size();
	first += second;

	//Check
	EXPECT_EQ(ExpectedSize, first.size());

	auto receivedByte = first.getByte(BYTE_0);
	EXPECT_TRUE(byte == receivedByte);

	receivedByte = first.getByte(BYTE_8 - 1);
	EXPECT_TRUE(byte == receivedByte);

	receivedByte = first.getByte(BYTE_8);
	EXPECT_TRUE(byte == receivedByte);

	receivedByte = first.getByte((2 * BYTE_8) - 1);
	EXPECT_TRUE(byte == receivedByte);
}

TEST_F(ByteArray_fixture, Add_and_assign_operator_for_Byte_8_add_to_empty_array)
{
	ByteArray byteArray(ZERO_BYTES);
	Byte_8 byte;

	//Trigger and Check
	byteArray += byte;
	EXPECT_EQ(ONE_BYTE, byteArray.size());
}

TEST_F(ByteArray_fixture, Add_and_assign_operator_for_Byte_8_add_to_not_empty_array)
{
	ByteArray byteArray(ONE_BYTE);
	Byte_8 byte;

	byte.setBit(BIT_0, true);

	//Trigger and Check
	byteArray += byte;
	EXPECT_EQ(TWO_BYTES, byteArray.size());
	EXPECT_EQ("0000000000000001", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, CropHead)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	bool result = byteArray.cropHead(ONE_BYTE);

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ(THREE_BYTES, byteArray.size());
	EXPECT_EQ("000000110000000000000111", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, CropHead_value_zero)
{
	ByteArray byteArray(FOUR_BYTES);

	//Trigger
	bool result = byteArray.cropHead(ZERO_BYTES);

	//Check
	EXPECT_FALSE(result);
	EXPECT_EQ(FOUR_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, CropHead_value_grater_than_size)
{
	ByteArray byteArray(FOUR_BYTES);

	//Trigger
	bool result = byteArray.cropHead(EIGHT_BYTES);

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, CropTail)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	bool result = byteArray.cropTail(ONE_BYTE);

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ(THREE_BYTES, byteArray.size());
	EXPECT_EQ("000000010000001100000000", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, CropTail_value_zero)
{
	ByteArray byteArray(FOUR_BYTES);

	//Trigger
	bool result = byteArray.cropTail(ZERO_BYTES);

	//Check
	EXPECT_FALSE(result);
	EXPECT_EQ(FOUR_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, CropTail_value_grater_than_size)
{
	ByteArray byteArray(FOUR_BYTES);

	//Trigger
	bool result = byteArray.cropTail(EIGHT_BYTES);

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Crop)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BIT_3);

	//Trigger
	bool result = byteArray.crop(ONE_BYTE, ONE_BYTE);

	//Check
	EXPECT_TRUE(result);
	EXPECT_EQ(TWO_BYTES, byteArray.size());
	EXPECT_EQ("0000001100000000", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, Crop_values_grater_than_size)
{
	//Trigger and Check
	ByteArray byteArray(FOUR_BYTES);

	bool result = byteArray.crop(ZERO_BYTES, FIVE_BYTES);
	EXPECT_TRUE(result);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());

	//Trigger and Check
	byteArray = ByteArray(FOUR_BYTES);

	result = byteArray.crop(FIVE_BYTES, ZERO_BYTES);
	EXPECT_TRUE(result);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Crop_values_equal_zero)
{
	//Trigger and Check
	ByteArray byteArray(FOUR_BYTES);

	bool result = byteArray.crop(ZERO_BYTES, ZERO_BYTES);
	EXPECT_FALSE(result);
	EXPECT_EQ(FOUR_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Crop_added_values_grater_than_size)
{
	ByteArray byteArray(FOUR_BYTES);

	//Trigger and Check
	bool result = byteArray.crop(THREE_BYTES, THREE_BYTES);

	EXPECT_TRUE(result);
	EXPECT_EQ(ZERO_BYTES, byteArray.size());
}

TEST_F(ByteArray_fixture, Slice)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	const int HeadIndex = 1;
	const int TailIndex = 2;
	ByteArray resultArray = byteArray.slice(HeadIndex, TailIndex);

	//Check
	EXPECT_EQ(TWO_BYTES, resultArray.size());
	EXPECT_EQ("0000001100000000", resultArray.getAsString());
}

TEST_F(ByteArray_fixture, Slice_wrong_index)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	const int HeadIndex = 1;
	const int TailIndex = 5;
	ByteArray resultArray = byteArray.slice(HeadIndex, TailIndex);

	//Check
	EXPECT_EQ(ZERO_BYTES, resultArray.size());
}

TEST_F(ByteArray_fixture, Slice_the_same_indexes)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	const int HeadIndex = 1;
	const int TailIndex = 1;
	ByteArray resultArray = byteArray.slice(HeadIndex, TailIndex);

	//Check
	EXPECT_EQ(ONE_BYTE, resultArray.size());
	EXPECT_EQ(byteArray.getByte(BYTE_1), resultArray.getByte(BYTE_0));
	EXPECT_EQ("00000011", resultArray.getAsString());	
}

TEST_F(ByteArray_fixture, Slice_wrong_index_order)
{
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	byte.setBit(BIT_1, true);
	byteArray.setByte(byte, BYTE_1);

	byte.setBit(BIT_2, true);
	byteArray.setByte(byte, BYTE_3);

	//Trigger
	const int HeadIndex = 2;
	const int TailIndex = 1;
	ByteArray resultArray = byteArray.slice(HeadIndex, TailIndex);

	//Check
	EXPECT_EQ(TWO_BYTES, resultArray.size());
	EXPECT_EQ("0000001100000000", resultArray.getAsString());
}

TEST_F(ByteArray_fixture, Operator_access_correct_value_read)
{
	ByteArray byteArray(TWO_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	//Trigger
	Byte_8 receivedByte = byteArray[BYTE_0];

	//Check
	EXPECT_EQ("00000001", receivedByte.getAsString());
}

TEST_F(ByteArray_fixture, Operator_access_correct_value_read_const)
{
	ByteArray byteArray(TWO_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);
	byteArray.setByte(byte, BYTE_0);

	//Trigger
	const ByteArray * ptr = &byteArray;
	Byte_8 receivedByte = (*ptr)[BYTE_0];

	//Check
	EXPECT_EQ("00000001", receivedByte.getAsString());
}

TEST_F(ByteArray_fixture, Operator_access_correct_value_set)
{
	ByteArray byteArray(TWO_BYTES);
	Byte_8 byte;

	byte.setBit(BIT_0, true);

	//Trigger
	byteArray[BYTE_0] = byte;

	//Check
	EXPECT_EQ("00000001", byteArray[BYTE_0].getAsString());
	EXPECT_EQ("0000000100000000", byteArray.getAsString());	
}

TEST_F(ByteArray_fixture, Add_sharp_operator_for_byte)
{
	ByteArray byteArray;
	const Byte_8 byte("11110001");

	byteArray << byte;

	EXPECT_EQ(ONE_BYTE, byteArray.size());
	EXPECT_EQ(byte, byteArray.getByte(BYTE_0));
}

TEST_F(ByteArray_fixture, Add_sharp_operator_cascaded_for_byte)
{
	ByteArray byteArray;
	const Byte_8 byte("11110001");

	byteArray << byte << byte << byte;

	EXPECT_EQ(THREE_BYTES, byteArray.size());
	EXPECT_EQ("111100011111000111110001", byteArray.getAsString());
}

TEST_F(ByteArray_fixture, Constructor_with_move_semantic)
{
	Byte_8 byte;
	byte.setBit(4);
	byte.setBit(5);

	ByteArray firstArray;
	firstArray << byte << byte << byte;

	ByteArray secondArray(std::move(firstArray));

	EXPECT_EQ(THREE_BYTES, secondArray.size());

	EXPECT_EQ(ZERO_BYTES, firstArray.size());
	EXPECT_TRUE(firstArray.isEmpty());
}

TEST_F(ByteArray_fixture, Move_assignment_operator)
{
	Byte_8 byte;
	byte.setBit(4);
	byte.setBit(5);

	ByteArray firstArray;
	ByteArray secondArray;

	firstArray << byte << byte << byte;

	secondArray = std::move(firstArray);

	EXPECT_EQ(THREE_BYTES, secondArray.size());

	EXPECT_EQ(ZERO_BYTES, firstArray.size());
	EXPECT_TRUE(firstArray.isEmpty());
}