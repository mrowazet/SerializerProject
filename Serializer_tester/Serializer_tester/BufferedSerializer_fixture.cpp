#include "BufferedSerializer_fixture.h"
#include "SerializableTestClass.h"

using namespace testing;
using namespace srl;

BufferedSerializer_fixture::BufferedSerializer_fixture()
{
}

BufferedSerializer_fixture::~BufferedSerializer_fixture()
{
}

void BufferedSerializer_fixture::SetUp()
{
}

void BufferedSerializer_fixture::TearDown()
{
	ut::cleanUpTestFilesAndDirectories();
}

srl::BufferedSerializerTestable BufferedSerializer_fixture::makeSerializerWithDefaultDirOpened(const srl::IOMode openMode)
{
	ut::createDefaultOutput();

	BufferedSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, openMode);

	[&](){ ASSERT_TRUE(serializer.isFileOpened()); }();

	return serializer;
}

srl::ByteArray BufferedSerializer_fixture::makeByteArray(const unsigned int & size, srl::Byte_8 byte)
{
	srl::ByteArray byteArray(size);

	for (auto& byteToFill : byteArray)
		byteToFill = byte;

	return byteArray;
}

TEST_F(BufferedSerializer_fixture, canBeConstructedWithMoveSemantic)
{
	auto bufferSize = 24;
	BufferedSerializer serializer(bufferSize);
	BufferedSerializer destSerializer(std::move(serializer));
}

TEST_F(BufferedSerializer_fixture, canBeAssignedWithMoveSemantic)
{
	BufferedSerializer serializer;
	BufferedSerializer destSerializer;

	destSerializer = std::move(serializer);
}

TEST_F(BufferedSerializer_fixture, BufferMock_can_be_instantiate)
{
	BufferMock bufferMock;
}

TEST_F(BufferedSerializer_fixture, default_consturtor_set_buffer_size_to_minimum)
{
	BufferedSerializer serializer;
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_value_if_in_range)
{
	auto bufferSize = 54;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(bufferSize, serializer.getBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_min_if_below_min)
{
	auto bufferSize = 15;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_max_if_grater_than_max)
{
	auto bufferSize = 257;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, serializer.getBufferSize());
}

TEST_F(BufferedSerializer_fixture, file_is_not_opened_when_default_constructor_is_used)
{
	BufferedSerializer serializer;
	EXPECT_FALSE(serializer.isFileOpened());
}

TEST_F(BufferedSerializer_fixture, clearBuffer_clears_internal_buffer)
{
	BufferedSerializerTestable serializer;
	auto& bufferMock = serializer.getBufferMock();

	EXPECT_CALL(bufferMock, clear());
	serializer.clearBuffer();
}

TEST_F(BufferedSerializer_fixture, clear_clears_buffer)
{
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, clear());
	serializer.clear();
}

TEST_F(BufferedSerializer_fixture, clear_does_not_call_clearBuffer_if_file_not_opened)
{
	BufferedSerializerTestable serializer;
	ASSERT_FALSE(serializer.isFileOpened());

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, clear()).Times(0);
	serializer.clear();
}

TEST_F(BufferedSerializer_fixture, getData_calls_data_on_buffer)
{
	BufferedSerializerTestable serializer;
	ByteArray internalBuffer;

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, data()).WillOnce(ReturnRef(internalBuffer));

	serializer.getData();
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_true_if_file_not_opened)
{
	BufferedSerializerTestable serializer;

	EXPECT_TRUE(serializer.isEmpty());
	EXPECT_TRUE(serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_true_if_opened_file_has_size_zero_and_buffer_is_empty)
{
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto fileSize = 0;
	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(fileSize));

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, isEmpty()).WillRepeatedly(Return(true));

	EXPECT_TRUE(serializer.isEmpty());
	EXPECT_TRUE(serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_file_size_is_grater_than_zero_and_buffer_is_empty)
{
	auto serializer = makeSerializerWithDefaultDirOpened(srl::IOMode::Append);
	
	auto fileSize = 10;
	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(fileSize));

	EXPECT_FALSE(serializer.isEmpty());
	EXPECT_FALSE(serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_opened_file_has_size_zero_but_buffer_is_not_empty)
{
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto fileSize = 0;
	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(fileSize));

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, isEmpty()).WillRepeatedly(Return(false));

	EXPECT_FALSE(serializer.isEmpty());
	EXPECT_FALSE(serializer);
}

TEST_F(BufferedSerializer_fixture, operators_to_read_write_should_trhow_error_if_file_is_not_opened)
{
	BufferedSerializerTestable serializer;

	ByteArray byteArray;
	ASSERT_THROW(serializer << byteArray, std::ios_base::failure);

	std::string testString = "test";
	ASSERT_THROW(serializer << testString, std::ios_base::failure);
	ASSERT_THROW(serializer >> testString, std::ios_base::failure);

	ASSERT_THROW(serializer << testString.c_str(), std::ios_base::failure);

	SerializableTestClass serializableObject;
	ASSERT_THROW(serializer << serializableObject, std::ios_base::failure);
	ASSERT_THROW(serializer >> serializableObject, std::ios_base::failure);
}

TEST_F(BufferedSerializer_fixture, ByteArray_is_added_to_buffer_if_size_is_lower_than_buffer_size_write_index_zero)
{
	auto byteArray = makeByteArray(SERIALIZER_BUFFER_MIN - 1);
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, write(byteArray));

	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, writeToFile(_, _)).Times(0);

	serializer << byteArray;
}

TEST_F(BufferedSerializer_fixture, Write_index_should_be_incremented_after_write_of_ByteArray)
{
	auto dataSize = 5u;
	auto byteArray = makeByteArray(dataSize);
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, write(testing::A<const ByteArray&>()));

	serializer << byteArray;

	EXPECT_EQ(dataSize, serializer.getWriteIndex());
}

TEST_F(BufferedSerializer_fixture, Set_get_indexes_works_correctly_within_range)
{
	auto serializer = makeSerializerWithDefaultDirOpened();

	ASSERT_EQ(0, serializer.getWriteIndex());
	ASSERT_EQ(0, serializer.getReadIndex());

	auto writeIndex = 4;
	auto readIndex = 7;

	EXPECT_TRUE(serializer.setWriteIndex(writeIndex));
	EXPECT_TRUE(serializer.setReadIndex(readIndex));

	EXPECT_EQ(writeIndex, serializer.getWriteIndex());
	EXPECT_EQ(readIndex, serializer.getReadIndex());
}

TEST_F(BufferedSerializer_fixture, Read_Write_indexes_are_zero_after_clear)
{
	auto serializer = makeSerializerWithDefaultDirOpened();
	const auto index = 7;

	ASSERT_TRUE(serializer.setReadIndex(index));
	ASSERT_TRUE(serializer.setWriteIndex(index));

	serializer.clear();

	EXPECT_EQ(0, serializer.getReadIndex());
	EXPECT_EQ(0, serializer.getWriteIndex());
}

TEST_F(BufferedSerializer_fixture, Set_indexes_should_return_false_if_file_not_opened)
{
	BufferedSerializer serializer;
	const auto index = 7;

	ASSERT_FALSE(serializer.setReadIndex(index));
	ASSERT_FALSE(serializer.setWriteIndex(index));
}

//TODO implement interface for file and testable class
//TEST_F(BufferedSerializer_fixture, Set_indexes_should_return_false_if_grater_than_file_size)
//{
//	auto serializer = makeSerializerWithDefaultDirOpened();
//
//	auto fileSize = 5;
//	auto& fileMock = serializer.getFileHandlingMock();
//	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(fileSize));
//
//	auto index = 6;
//	EXPECT_FALSE(serializer.setReadIndex(index));
//	EXPECT_FALSE(serializer.setWriteIndex(index));
//}
