#include "Buffer_fixture.h"

using namespace testing;
using namespace srl;

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

TEST_F(Buffer_fixture, consturctor_set_bufferSize)
{
	auto SIZE = 22u;
	Buffer buffer(SIZE);

	EXPECT_EQ(SIZE, buffer.size());
}

TEST_F(Buffer_fixture, default_constructor_set_buffer_size_for_min)
{
	Buffer buffer;

	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.size());
}

TEST_F(Buffer_fixture, can_be_constructed_with_move_semantic)
{
	Buffer sourceBuffer(SERIALIZER_BUFFER_MAX);
	Buffer destinationBuffer(std::move(sourceBuffer));

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, destinationBuffer.size());
}

TEST_F(Buffer_fixture, source_buffer_should_be_empty_and_resized_after_move)
{
	Buffer sourceBuffer(SERIALIZER_BUFFER_MAX);
	//TODO add data to source buffer
	Buffer destinationBuffer(std::move(sourceBuffer));

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, sourceBuffer.size());
	EXPECT_TRUE(sourceBuffer.isEmpty());
}

TEST_F(Buffer_fixture, can_be_correctly_constructed_with_copy_constructor)
{
	//TODO
}

TEST_F(Buffer_fixture, can_be_correctly_assigned_by_copy)
{
	//TODO
}

TEST_F(Buffer_fixture, can_be_assign_with_move_semantic)
{
	Buffer sourceBuffer(SERIALIZER_BUFFER_MIN);
	Buffer destinationBuffer(SERIALIZER_BUFFER_MAX);

	destinationBuffer = std::move(sourceBuffer);
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, destinationBuffer.size());
}

TEST_F(Buffer_fixture, getData_returns_correct_address)
{
	BufferTestable buffer(SERIALIZER_BUFFER_MIN);

	auto data = buffer.data();
	EXPECT_EQ(buffer.internalData[0], data);
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
	Buffer buffer;
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

TEST_F(Buffer_fixture, clear_reset_m_position)
{
	BufferTestable buffer;
	buffer.m_index = 3;

	buffer.clear();

	auto expectedPosition = 0;
	EXPECT_EQ(expectedPosition, buffer.m_index);
}

TEST_F(Buffer_fixture, internal_buffer_is_the_same_size_after_clear)
{
	Buffer buffer(SERIALIZER_BUFFER_MIN);

	buffer.clear();
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.size());
}

TEST_F(Buffer_fixture, write_byte_stores_data_in_internal_buffer)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	EXPECT_EQ(byte, buffer.internalData[0]);
}

TEST_F(Buffer_fixture, write_byte_increment_position_by_1)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	auto expectedPosition = 1u;
	EXPECT_EQ(expectedPosition, buffer.m_index);
}

TEST_F(Buffer_fixture, read_byte_returns_last_stored_byte)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	auto loadedByte = buffer.read();
	EXPECT_EQ(byte, loadedByte);
}

TEST_F(Buffer_fixture, read_byte_decrement_position_by_1)
{
	Byte_8 byte("11111111");
	BufferTestable buffer;
	buffer.write(byte);

	buffer.read();

	auto expectedPosition = 0u;
	EXPECT_EQ(expectedPosition, buffer.m_index);
}