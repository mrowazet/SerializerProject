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
	ASSERT_EQ(0, serializer.getFileSize());

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, isEmpty()).WillRepeatedly(Return(true));

	EXPECT_TRUE(serializer.isEmpty());
	EXPECT_TRUE(serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_file_size_is_grater_than_zero_and_buffer_is_empty)
{
	auto serializer = makeSerializerWithDefaultDirOpened(srl::IOMode::Append);
	ASSERT_LT(0, serializer.getFileSize());

	EXPECT_FALSE(serializer.isEmpty());
	EXPECT_FALSE(serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_opened_file_has_size_zero_but_buffer_is_not_empty)
{
	auto serializer = makeSerializerWithDefaultDirOpened();
	ASSERT_EQ(0, serializer.getFileSize());

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

TEST_F(BufferedSerializer_fixture, ByteArray_is_added_to_buffer_if_size_is_lower_than_buffer_size)
{

}
