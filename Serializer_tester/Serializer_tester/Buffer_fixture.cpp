#include "Buffer_fixture.h"

using namespace testing;
using namespace srl;

namespace
{
	const unsigned int INDEX_ZERO = 0u;
}

Buffer_fixture::Buffer_fixture()
{
}

Buffer_fixture::~Buffer_fixture()
{

}

void Buffer_fixture::SetUp()
{

}

void Buffer_fixture::TearDown()
{

}

srl::ByteArray Buffer_fixture::makeByteArray()
{
	auto sizeInBytes = 16u;
	ByteArray byteArray(sizeInBytes);
	Byte_8 byte("01010101");

	for (int i = 0; i < EIGHT_BYTES; i++)
		byteArray.setByte(byte, i);
				
	const std::string expectedFirstEightBytes = "0101010101010101010101010101010101010101010101010101010101010101";

	auto arrayAsString = byteArray.getAsString();
	arrayAsString.resize(EIGHT_BYTES * Byte_8::BITS_IN_BYTE);

	EXPECT_EQ(expectedFirstEightBytes, arrayAsString);

	return byteArray;
}

TEST_F(Buffer_fixture, consturctor_set_bufferSize)
{
	auto SIZE = 22u;
	BufferTestable buffer(SIZE);

	EXPECT_EQ(SIZE, buffer.size());
}

TEST_F(Buffer_fixture, default_constructor_set_buffer_size_for_min)
{
	BufferTestable buffer;

	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.size());
}

TEST_F(Buffer_fixture, can_be_constructed_with_move_semantic)
{
	BufferTestable sourceBuffer(SERIALIZER_BUFFER_MAX);
	BufferTestable destinationBuffer(std::move(sourceBuffer));

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, destinationBuffer.size());
}

TEST_F(Buffer_fixture, source_buffer_should_be_empty_and_resized_after_move)
{
	BufferTestable sourceBuffer(SERIALIZER_BUFFER_MAX);
	Byte_8 byte("11111111");
	sourceBuffer.internalData[0] = byte;

	BufferTestable destinationBuffer(std::move(sourceBuffer));

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, sourceBuffer.size());
	EXPECT_TRUE(sourceBuffer.isEmpty());
}

TEST_F(Buffer_fixture, can_be_correctly_constructed_with_copy_constructor)
{
	BufferTestable orginalBuffer(SERIALIZER_BUFFER_MIN);
	Byte_8 byte("11111111");
	orginalBuffer.internalData[1] = byte;

	BufferTestable copiedBuffer(orginalBuffer);

	EXPECT_EQ(orginalBuffer.size(), copiedBuffer.size());
	EXPECT_EQ(orginalBuffer.isEmpty(), copiedBuffer.isEmpty());
	EXPECT_EQ(orginalBuffer.internalData, copiedBuffer.internalData);
}

TEST_F(Buffer_fixture, can_be_correctly_assigned_by_copy)
{
	BufferTestable orginalBuffer(SERIALIZER_BUFFER_MIN);
	Byte_8 byte("11111111");
	orginalBuffer.internalData[1] = byte;

	BufferTestable copiedBuffer(SERIALIZER_BUFFER_MAX);
	copiedBuffer.internalData[0] = byte;
	copiedBuffer = orginalBuffer;

	EXPECT_EQ(orginalBuffer.size(), copiedBuffer.size());
	EXPECT_EQ(orginalBuffer.isEmpty(), copiedBuffer.isEmpty());
	EXPECT_EQ(orginalBuffer.internalData, copiedBuffer.internalData);
}

TEST_F(Buffer_fixture, can_be_assign_with_move_semantic)
{
	BufferTestable sourceBuffer(SERIALIZER_BUFFER_MIN);
	BufferTestable destinationBuffer(SERIALIZER_BUFFER_MAX);

	destinationBuffer = std::move(sourceBuffer);
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, destinationBuffer.size());
}

TEST_F(Buffer_fixture, getData_returns_correct_address)
{
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	auto data = buffer.data();
	EXPECT_EQ(buffer.internalData, data);
}

TEST_F(Buffer_fixture, isEmpty_returns_false_if_any_bit_is_1)
{
	const Byte_8 BYTE("00000001");
	BufferTestable buffer;
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	EXPECT_FALSE(buffer.isEmpty());
}

TEST_F(Buffer_fixture, isEmpty_returns_true_if_all_bits_are_0)
{
	BufferTestable buffer;
	EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(Buffer_fixture, boolOperator_returns_same_values_as_isEmpty)
{
	BufferTestable buffer;

	EXPECT_TRUE(buffer);

	const Byte_8 BYTE("00000001");
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	EXPECT_FALSE(buffer);
}

TEST_F(Buffer_fixture, clear_erase_all_data_in_internal_buffer)
{
	const Byte_8 BYTE("01010101");
	BufferTestable buffer;
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	ASSERT_FALSE(buffer.isEmpty());

	buffer.clear();
	EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(Buffer_fixture, clear_reset_read_and_write_indexes)
{
	BufferTestable buffer;
	buffer.m_readIndex = 3;
	buffer.m_writeIndex = 6;

	buffer.clear();

	auto expectedPosition = 0;
	EXPECT_EQ(expectedPosition, buffer.m_readIndex);
	EXPECT_EQ(expectedPosition, buffer.m_writeIndex);
}

TEST_F(Buffer_fixture, internal_buffer_is_the_same_size_after_clear)
{
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	buffer.clear();
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.size());
}

TEST_F(Buffer_fixture, should_clear_access_indexes_but_data_should_not_be_zeroed_after_call_clearIndexes)
{
	const Byte_8 BYTE("01010101");
	ByteArray byteArray;
	byteArray << BYTE << BYTE << BYTE;

	BufferTestable buffer;
	buffer.write(byteArray);
	buffer.read();

	ASSERT_FALSE(buffer.isEmpty());

	buffer.clearIndexes();

	EXPECT_FALSE(buffer.isEmpty());
	EXPECT_EQ(INDEX_ZERO, buffer.getReadIndex());
	EXPECT_EQ(INDEX_ZERO, buffer.getWriteIndex());
}

TEST_F(Buffer_fixture, write_byte_stores_data_in_internal_buffer)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	EXPECT_EQ(byte, buffer.internalData[0]);
}

TEST_F(Buffer_fixture, write_byte_increment_write_indexes_by_1)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	auto expectedWritePosition = 1u;
	auto expectedReadPosition = 0u;
	EXPECT_EQ(expectedWritePosition, buffer.m_writeIndex);
	EXPECT_EQ(expectedReadPosition, buffer.m_readIndex);
}

TEST_F(Buffer_fixture, read_byte_returns_firstly_stored_byte)
{
	Byte_8 byte("11111111");
	Byte_8 anotherByte("01010101");
	BufferTestable buffer;

	buffer.write(byte);
	buffer.write(anotherByte);

	auto loadedByte = buffer.read();
	EXPECT_EQ(byte, loadedByte);
}

TEST_F(Buffer_fixture, read_byte_incerment_read_position_by_1)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	
	buffer.write(byte);
	buffer.write(byte);
	buffer.read();

	auto expectedWritePosition = 2u;
	auto expectedReadPosition = 1u;
	EXPECT_EQ(expectedWritePosition, buffer.m_writeIndex);
	EXPECT_EQ(expectedReadPosition, buffer.m_readIndex);
}

TEST_F(Buffer_fixture, get_set_read_index)
{
	BufferTestable buffer;
	auto index = 8;

	buffer.setReadIndex(index);
	EXPECT_EQ(index, buffer.getReadIndex());
}

TEST_F(Buffer_fixture, get_set_write_index)
{
	BufferTestable buffer;
	auto index = 8;

	buffer.setWriteIndex(index);
	EXPECT_EQ(index, buffer.getWriteIndex());
}

TEST_F(Buffer_fixture, data_from_ByteArray_can_be_write_to_empty_buffer)
{
	const auto byteArray = makeByteArray();
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	ASSERT_TRUE(buffer.isEmpty());

	buffer.write(byteArray);
	EXPECT_EQ(byteArray.getAsString(), buffer.internalData.getAsString());
}

TEST_F(Buffer_fixture, write_ByteArray_increment_write_index_by_array_size)
{
	const auto byteArray = makeByteArray();
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	auto indexZero = 0u;
	ASSERT_EQ(indexZero, buffer.getWriteIndex());

	auto expectedWriteIndex = buffer.getWriteIndex() + byteArray.size();

	buffer.write(byteArray);	
	EXPECT_EQ(expectedWriteIndex, buffer.getWriteIndex());
}

TEST_F(Buffer_fixture, data_from_ByteArray_can_be_write_to_not_empty_buffer)
{
	const auto byteArray = makeByteArray();

	auto doubledMinSize = 2 * SERIALIZER_BUFFER_MIN;
	BufferTestable buffer(doubledMinSize);

	buffer.write(byteArray);
	ASSERT_FALSE(buffer.isEmpty());

	auto expectedArray = byteArray + byteArray;
	buffer.write(byteArray);
	EXPECT_EQ(expectedArray.getAsString(), buffer.internalData.getAsString());
}

TEST_F(Buffer_fixture, data_can_be_read_as_ByteArray_from_buffer)
{
	const auto byteArray = makeByteArray();
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	buffer.internalData = byteArray;

	auto loadedData = buffer.read(SERIALIZER_BUFFER_MIN);

	EXPECT_EQ(byteArray, loadedData);
}

TEST_F(Buffer_fixture, read_as_ByteArray_increments_readIndex)
{
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	buffer.read(SERIALIZER_BUFFER_MIN);

	auto expectedReadIndex = SERIALIZER_BUFFER_MIN;
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.getReadIndex());
}

TEST_F(Buffer_fixture, unsigned_int_is_correcty_stored_in_buffer)
{
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	auto testUInt = 13u;
	buffer.write(testUInt);

	auto storedValue = reinterpret_cast<const unsigned int*>(&buffer.internalData[0]);

	EXPECT_EQ(testUInt, *storedValue);
	EXPECT_EQ(BYTE_4, buffer.getWriteIndex());
}
