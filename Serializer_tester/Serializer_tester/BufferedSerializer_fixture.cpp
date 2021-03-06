#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"
#include "SerializableTestClass.h"
#include "BufferedSerializerTestable.h"

using namespace testing;
using namespace srl;

namespace
{
	const unsigned int SIZE_WITHIN_RANGE	= 54u;
	const unsigned int SZIE_LOWER_THAN_MIN	= 15u;
	const unsigned int SIZE_GRATER_THAN_MAX = 257u;

	const unsigned int EMPTY_FILE_SIZE		= 0u;
	const unsigned int FILE_SIZE			= 12u;

	const unsigned int INDEX_ZERO			= 0u;
	const unsigned int WRITE_INDEX			= 4u;
	const unsigned int READ_INDEX			= 7u;

	const unsigned int DATA_SIZE_5			= 5u;
	const unsigned int DATA_SIZE_10			= 10u;
	const unsigned int DATA_SIZE_15			= 15u;
	const unsigned int DATA_SIZE_20			= 20u;
}

class BufferedSerializer_Base : public ::testing::Test
{
public:
	BufferedSerializer_Base();
	~BufferedSerializer_Base() = default;

	virtual void SetUp() override;
	virtual void TearDown() override;

protected:
	const srl::ByteArray BYTE_ARRAY_5;
	const srl::ByteArray BYTE_ARRAY_10;
	const srl::ByteArray BYTE_ARRAY_15;
	const srl::ByteArray BYTE_ARRAY_20;

	srl::BufferedSerializerTestable m_serializer;
	srl::BufferMock& m_bufferMock;
	srl::FileHandlingMock& m_fileHandlingMock;
	srl::BufferedDataInfo& m_bufferInfo;

	srl::BufferedSerializerTestable makeSerializerWithDefaultDirOpened(const srl::IOMode openMode = srl::IOMode::Truncate);
	srl::ByteArray makeByteArray(const unsigned int& size, srl::Byte_8 byte = srl::Byte_8("10101010"));
	void closeFile();
};

BufferedSerializer_Base::BufferedSerializer_Base()
	:m_serializer(makeSerializerWithDefaultDirOpened()),
	 m_bufferMock(m_serializer.getBufferMock()),
	 m_fileHandlingMock(m_serializer.getFileHandlingMock()),
	 m_bufferInfo(m_serializer.getBufferDataInfo()),
	 BYTE_ARRAY_5(makeByteArray(DATA_SIZE_5)),
	 BYTE_ARRAY_10(makeByteArray(DATA_SIZE_10)),
	 BYTE_ARRAY_15(makeByteArray(DATA_SIZE_15)),
	 BYTE_ARRAY_20(makeByteArray(DATA_SIZE_20))
{

}

void BufferedSerializer_Base::SetUp()
{

}

void BufferedSerializer_Base::TearDown()
{
	m_serializer.closeFile();
	ut::cleanUpTestFilesAndDirectories();
}

srl::BufferedSerializerTestable BufferedSerializer_Base::makeSerializerWithDefaultDirOpened(const srl::IOMode openMode)
{
	ut::createDefaultOutput();

	BufferedSerializerTestable serializer(SERIALIZER_BUFFER_MIN);
	serializer.openFile(DEFAULT_DIRECTORY, openMode);

	[&]() { ASSERT_TRUE(serializer.isFileOpened()); }();

	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(FILE_SIZE));

	return serializer;
}

srl::ByteArray BufferedSerializer_Base::makeByteArray(const unsigned int & size, srl::Byte_8 byte)
{
	srl::ByteArray byteArray(size);

	for (auto& byteToFill : byteArray)
		byteToFill = byte;

	return byteArray;
}

void BufferedSerializer_Base::closeFile()
{
	m_serializer.closeFile();
	ASSERT_FALSE(m_serializer.isFileOpened());
}

class BufferedSerializer_fixture : public BufferedSerializer_Base
{
public:
	BufferedSerializer_fixture() = default;
	~BufferedSerializer_fixture() = default;

protected:

};

TEST_F(BufferedSerializer_fixture, canBeConstructedWithMoveSemantic)
{
	BufferedSerializer serializer;
	BufferedSerializer destSerializer(std::move(serializer));
}

TEST_F(BufferedSerializer_fixture, canBeAssignedWithMoveSemantic)
{
	BufferedSerializer serializer;
	BufferedSerializer destSerializer;

	destSerializer = std::move(serializer);
}

TEST_F(BufferedSerializer_fixture, default_consturtor_set_buffer_size_to_minimum)
{
	BufferedSerializer serializer;
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_value_if_in_range)
{
	BufferedSerializer serializer(SIZE_WITHIN_RANGE);
	EXPECT_EQ(SIZE_WITHIN_RANGE, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_min_if_below_min)
{
	BufferedSerializer serializer(SZIE_LOWER_THAN_MIN);
	EXPECT_EQ(SERIALIZER_BUFFER_MIN, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, maxBufferSize_is_set_to_max_if_grater_than_max)
{
	BufferedSerializer serializer(SIZE_GRATER_THAN_MAX);
	EXPECT_EQ(SERIALIZER_BUFFER_MAX, serializer.getMaxBufferSize());
}

TEST_F(BufferedSerializer_fixture, file_is_not_opened_when_default_constructor_is_used)
{
	BufferedSerializer serializer;
	EXPECT_FALSE(serializer.isFileOpened());
}

TEST_F(BufferedSerializer_fixture, clearBuffer_clears_internal_buffer)
{
	EXPECT_CALL(m_bufferMock, clear());
	m_serializer.clearBuffer();
}

TEST_F(BufferedSerializer_fixture, clearBuffer_clears_access_indexes)
{
	m_bufferInfo.updateAccessIndexesByAddedDataSize(DATA_SIZE_5);
	ASSERT_FALSE(m_bufferInfo.areAccessIndexesCleared());
	EXPECT_CALL(m_bufferMock, clear());

	m_serializer.clearBuffer();
	EXPECT_TRUE(m_bufferInfo.areAccessIndexesCleared());
}

TEST_F(BufferedSerializer_fixture, clear_clears_buffer)
{
	EXPECT_CALL(m_bufferMock, clear());
	m_serializer.clear();
}

TEST_F(BufferedSerializer_fixture, clear_does_not_call_clearBuffer_if_file_not_opened)
{
	closeFile();
	EXPECT_CALL(m_bufferMock, clear()).Times(0);

	m_serializer.clear();
}

TEST_F(BufferedSerializer_fixture, getData_calls_data_on_buffer)
{
	ByteArray internalBuffer;
	EXPECT_CALL(m_bufferMock, data()).WillOnce(ReturnRef(internalBuffer));

	m_serializer.getData();
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_true_if_file_not_opened)
{
	closeFile();

	EXPECT_TRUE(m_serializer.isEmpty());
	EXPECT_TRUE(m_serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_true_if_opened_file_has_size_zero_and_buffer_is_empty)
{
	EXPECT_CALL(m_fileHandlingMock, getFileSize()).WillRepeatedly(Return(EMPTY_FILE_SIZE));
	EXPECT_CALL(m_bufferMock, isEmpty()).WillRepeatedly(Return(true));

	EXPECT_TRUE(m_serializer.isEmpty());
	EXPECT_TRUE(m_serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_file_size_is_grater_than_zero_and_buffer_is_empty)
{
	EXPECT_CALL(m_fileHandlingMock, getFileSize()).WillRepeatedly(Return(DATA_SIZE_5));

	EXPECT_FALSE(m_serializer.isEmpty());
	EXPECT_FALSE(m_serializer);
}

TEST_F(BufferedSerializer_fixture, isEmpty_returns_false_if_opened_file_has_size_zero_but_buffer_is_not_empty)
{
	EXPECT_CALL(m_fileHandlingMock, getFileSize()).WillRepeatedly(Return(EMPTY_FILE_SIZE));
	EXPECT_CALL(m_bufferMock, isEmpty()).WillRepeatedly(Return(false));

	EXPECT_FALSE(m_serializer.isEmpty());
	EXPECT_FALSE(m_serializer);
}

TEST_F(BufferedSerializer_fixture, operators_to_read_write_should_trhow_error_if_file_is_not_opened)
{
	closeFile();

	ByteArray byteArray;
	ASSERT_THROW(m_serializer << byteArray, std::ios_base::failure);

	std::string testString = "test";
	ASSERT_THROW(m_serializer << testString, std::ios_base::failure);
	ASSERT_THROW(m_serializer >> testString, std::ios_base::failure);

	ASSERT_THROW(m_serializer << testString.c_str(), std::ios_base::failure);

	SerializableTestClass serializableObject;
	ASSERT_THROW(m_serializer << serializableObject, std::ios_base::failure);
	ASSERT_THROW(m_serializer >> serializableObject, std::ios_base::failure);
}

TEST_F(BufferedSerializer_fixture, Set_get_indexes_works_correctly_within_range)
{
	ASSERT_EQ(INDEX_ZERO, m_serializer.getWriteIndex());
	ASSERT_EQ(INDEX_ZERO, m_serializer.getReadIndex());

	EXPECT_TRUE(m_serializer.setWriteIndex(WRITE_INDEX));
	EXPECT_TRUE(m_serializer.setReadIndex(READ_INDEX));

	EXPECT_EQ(WRITE_INDEX, m_serializer.getWriteIndex());
	EXPECT_EQ(READ_INDEX, m_serializer.getReadIndex());
}

TEST_F(BufferedSerializer_fixture, Read_Write_indexes_are_zero_after_clear)
{
	ASSERT_TRUE(m_serializer.setWriteIndex(WRITE_INDEX));
	ASSERT_TRUE(m_serializer.setReadIndex(READ_INDEX));

	EXPECT_CALL(m_bufferMock, clear());
	m_serializer.clear();

	EXPECT_EQ(INDEX_ZERO, m_serializer.getReadIndex());
	EXPECT_EQ(INDEX_ZERO, m_serializer.getWriteIndex());
}

TEST_F(BufferedSerializer_fixture, Set_indexes_should_return_false_if_file_not_opened)
{
	m_serializer.closeFile();
	ASSERT_FALSE(m_serializer.isFileOpened());

	ASSERT_FALSE(m_serializer.setWriteIndex(WRITE_INDEX));
	ASSERT_FALSE(m_serializer.setReadIndex(READ_INDEX));
}

TEST_F(BufferedSerializer_fixture, Clear_clears_bufferInfo)
{
	auto data = makeByteArray(DATA_SIZE_5);

	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	EXPECT_CALL(m_bufferMock, clear());

	m_serializer << data;
	m_serializer.clear();

	EXPECT_TRUE(m_bufferInfo.isCleared());
}

TEST_F(BufferedSerializer_fixture, Set_write_index_should_return_false_if_grater_than_file_size)
{
	EXPECT_FALSE(m_serializer.setWriteIndex(FILE_SIZE + 1));
}

TEST_F(BufferedSerializer_fixture, Set_write_index_should_return_true_if_equal_of_lower_than_file_size)
{
	EXPECT_TRUE(m_serializer.setWriteIndex(FILE_SIZE));
	EXPECT_TRUE(m_serializer.setWriteIndex(FILE_SIZE - 1));
}

TEST_F(BufferedSerializer_fixture, Set_read_index_should_return_false_if_equal_grater_than_file_size)
{
	EXPECT_FALSE(m_serializer.setReadIndex(FILE_SIZE));
	EXPECT_FALSE(m_serializer.setReadIndex(FILE_SIZE + 1));
}

TEST_F(BufferedSerializer_fixture, Set_read_index_should_return_true_if_lower_than_file_size)
{
	EXPECT_TRUE(m_serializer.setReadIndex(FILE_SIZE - 1));
}

TEST_F(BufferedSerializer_fixture, Set_read_write_should_return_false_if_lower_than_zero)
{
	auto negativeValue = -1;

	EXPECT_FALSE(m_serializer.setWriteIndex(negativeValue));
	EXPECT_FALSE(m_serializer.setReadIndex(negativeValue));
}

class TestWriteData_fixture : public BufferedSerializer_fixture
{
public:
	TestWriteData_fixture() = default;

};

TEST_F(TestWriteData_fixture, Do_not_buffer_data_if_size_grater_than_buffer)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>())).Times(0);
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_20));

	m_serializer << BYTE_ARRAY_20;
}

TEST_F(TestWriteData_fixture, Access_indexes_should_be_clear_after_write_data_grater_than_buffer)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_10.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_10));

	m_serializer << BYTE_ARRAY_10;

	ASSERT_FALSE(m_bufferInfo.areAccessIndexesCleared());
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_20));

	m_serializer << BYTE_ARRAY_20;
	EXPECT_TRUE(m_bufferInfo.areAccessIndexesCleared());
}

TEST_F(TestWriteData_fixture, Buffer_begin_index_should_be_updated_after_write_data_grater_than_buffer)
{
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_20));

	m_serializer << BYTE_ARRAY_20;
	EXPECT_EQ(DATA_SIZE_20, m_bufferInfo.getBeginIndexRelativelyToFile());
}

TEST_F(TestWriteData_fixture, Write_index_should_be_incremented_after_write_data_grater_than_buffer)
{
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_20));

	m_serializer << BYTE_ARRAY_20;
	EXPECT_EQ(DATA_SIZE_20, m_serializer.getWriteIndex());
}

TEST_F(TestWriteData_fixture, ByteArray_is_added_to_buffer_if_size_is_lower_than_buffer_size_write_index_zero)
{
	EXPECT_CALL(m_bufferMock, write(BYTE_ARRAY_10));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);

	m_serializer << BYTE_ARRAY_10;
}

TEST_F(TestWriteData_fixture, Write_index_should_be_incremented_after_write_of_ByteArray)
{
	EXPECT_CALL(m_bufferMock, write(testing::A<const ByteArray&>()));

	m_serializer << BYTE_ARRAY_10;

	EXPECT_EQ(BYTE_ARRAY_10.size(), m_serializer.getWriteIndex());
}

TEST_F(TestWriteData_fixture, Last_correct_indexes_updated_after_write_to_buffer)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << BYTE_ARRAY_10;

	EXPECT_NE(INDEX_ZERO, m_bufferInfo.getLastCorrectWriteIndex());
	EXPECT_NE(INDEX_ZERO, m_bufferInfo.getLastCorrectReadIndex());
}

TEST_F(TestWriteData_fixture, Data_should_be_added_to_buffer_if_enough_space_available)
{
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>())).Times(3);
	m_serializer << BYTE_ARRAY_5 << BYTE_ARRAY_5 << BYTE_ARRAY_5;
}

TEST_F(TestWriteData_fixture, BufferInfo_should_be_updated_after_data_addition)
{
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>())).Times(3);
	m_serializer << BYTE_ARRAY_5 << BYTE_ARRAY_5 << BYTE_ARRAY_5;

	auto expectedLastWriteIndex = 15u;
	auto expectedLastReadIndex = 14u;
	EXPECT_EQ(expectedLastWriteIndex, m_bufferInfo.getLastCorrectWriteIndex());
	EXPECT_EQ(expectedLastReadIndex, m_bufferInfo.getLastCorrectReadIndex());
}

TEST_F(TestWriteData_fixture, Flush_should_write_data_from_buffer)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << BYTE_ARRAY_10;

	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_10.cbegin()));

	auto expectedDataAddress = reinterpret_cast<const char*>(&(*BYTE_ARRAY_10.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(expectedDataAddress, DATA_SIZE_10));
	m_serializer.flush();
}

TEST_F(TestWriteData_fixture, Flush_should_clear_access_indexes_and_set_begin_to_writeIndex)
{
	ASSERT_TRUE(m_bufferInfo.areAccessIndexesCleared());
	m_bufferInfo.updateAccessIndexesByAddedDataSize(DATA_SIZE_5);

	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));

	m_serializer << BYTE_ARRAY_5;

	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_5.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _));

	m_serializer.flush();
	EXPECT_TRUE(m_bufferInfo.areAccessIndexesCleared());
	EXPECT_EQ(DATA_SIZE_5, m_bufferInfo.getBeginIndexRelativelyToFile());
}

TEST_F(TestWriteData_fixture, Data_from_buffer_should_be_flushed_correctly_after_write_index_changed)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << BYTE_ARRAY_10;

	m_serializer.setWriteIndex(WRITE_INDEX);

	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_10.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, WRITE_INDEX));

	m_serializer.flush();
}

TEST_F(TestWriteData_fixture, Data_from_buffer_should_be_flushed_if_next_element_exceed_available_space)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << BYTE_ARRAY_10;

	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_10.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_10));

	auto sizeOfTheArrayWrittenToBuffer = 0u;
	{
		InSequence seq;
			
		EXPECT_CALL(m_bufferMock, clearIndexes());
		EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()))
			.WillOnce(Invoke([&](const auto& byteArray) {sizeOfTheArrayWrittenToBuffer = byteArray.size(); }));		
	}

	m_serializer << BYTE_ARRAY_15;

	EXPECT_EQ(DATA_SIZE_15, sizeOfTheArrayWrittenToBuffer);
	EXPECT_EQ(DATA_SIZE_10, m_bufferInfo.getBeginIndexRelativelyToFile());
	EXPECT_EQ(DATA_SIZE_15, m_bufferInfo.getLastCorrectWriteIndex());
	EXPECT_EQ(DATA_SIZE_15 - 1, m_bufferInfo.getLastCorrectReadIndex());
}

TEST_F(TestWriteData_fixture, Data_should_be_flushed_and_next_element_should_be_write_directly_to_file_when_data_exceeds_buffer_size)
{
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << BYTE_ARRAY_10;

	EXPECT_CALL(m_bufferMock, cbegin()).WillOnce(Return(BYTE_ARRAY_10.cbegin()));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_10));

	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, BYTE_ARRAY_20.size()));
	m_serializer << BYTE_ARRAY_20;

	auto expectedWriteIndex = DATA_SIZE_10 + BYTE_ARRAY_20.size();
	EXPECT_EQ(expectedWriteIndex, m_serializer.getWriteIndex());
}
/*TODO implement

* flush() will fail if:
	- begin index relatively to file is for example 50
	- then setWrite index is called with for example 40
	- at the moment setWrite does not trigger buffer reload so size of data to flush is -10!!!
	- reload for 'set' can be implemented as 'lazy'
	- consider also setRead function!
*/

class BufferedSerializer_functionalTest : public BufferedSerializer_Base
{
public:
	BufferedSerializer_functionalTest() = default;

protected:
	srl::BufferedSerializer m_realSerailizer;

};

//Dev purpose only - should be deleted when read function is ready
//TEST_F(BufferedSerializer_functionalTest, writeAndRead)
//{
//	EXPECT_TRUE(m_realSerailizer.openFile("a.bin"));
//	m_realSerailizer.clear();
//
//	ByteArray array(BYTE_ARRAY_10);
//	m_realSerailizer << array;
//
//	m_realSerailizer.flush();
//	m_realSerailizer.closeFile();
//}
