#include "BufferedSerializer_fixture.h"

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

TEST_F(BufferedSerializer_fixture, CyclicBufferMock_can_be_instantiate)
{
	CyclicBufferMock cyclicBufferMock;
}

TEST_F(BufferedSerializer_fixture, default_consturtor_set_buffer_size_to_minimum)
{
	BufferedSerializer serializer;
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_value_if_in_range)
{
	auto bufferSize = 54;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(bufferSize, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_min_if_below_min)
{
	auto bufferSize = 15;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_max_if_grater_than_max)
{
	auto bufferSize = 257;
	BufferedSerializer serializer(bufferSize);

	EXPECT_EQ(SERIALIZER_BUFFER_MAX, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, file_is_not_opened_when_default_constructor_is_used)
{
	BufferedSerializer serializer;
	EXPECT_FALSE(serializer.isFileOpened());
}

/*
TEST_F(BufferedSerializer_fixture, buffer_size_equal_to_file_size_when_file_size_is_less_than_buffer_max_size)
{   THIS TC IS WRONG
	BUFFER SIZE IS CONSTANT!
	ut::createDefaultOutput();
	const auto FileSize = fs::file_size(DEFAULT_DIRECTORY);
	const auto MaxBufferSize = 128;

	ASSERT_GT(MaxBufferSize, FileSize);

	BufferedSerializer serializer(DEFAULT_DIRECTORY, IOMode::Append, MaxBufferSize);
	ASSERT_TRUE(serializer.isFileOpened());

	EXPECT_EQ(FileSize, serializer.getBufferSize());
}
*/
TEST_F(BufferedSerializer_fixture, clearBuffer_clears_internal_buffer)
{
	BufferedSerializerTestable serializer;
	auto& mock = serializer.getCyclicBufferMock();

	EXPECT_CALL(mock, clear());
	serializer.clearBuffer();
}
/*
//TODO clear from ActiveSerializer::clearBase(); is unsafe! correct that and write test!
TEST_F(BufferedSerializer_fixture, clear_clears_buffer)
{
	ut::createDefaultOutput();

	BufferedSerializer serializer(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());
	ASSERT_GT(serializer.getBufferSize(), 0); //TODO

	serializer.clear();
	EXPECT_EQ(0, serializer.getBufferSize());
}
*/

/*
add internal indexes and do not forget to clear them with clear()!
*/