#include "BufferedSerializer_fixture.h"
#include "SerializableTestClass.h"

using namespace testing;
using namespace srl;

namespace
{

}

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

	BufferedSerializerTestable serializer(SERIALIZER_BUFFER_MIN);
	serializer.openFile(DEFAULT_DIRECTORY, openMode);

	[&](){ ASSERT_TRUE(serializer.isFileOpened()); }();

	auto& fileMock = serializer.getFileHandlingMock();
	EXPECT_CALL(fileMock, getFileSize()).WillRepeatedly(Return(SERIALIZER_BUFFER_MIN));

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

TEST_F(BufferedSerializer_fixture, clearBuffer_clears_internal_buffer)
{
	BufferedSerializerTestable serializer;
	auto& bufferMock = serializer.getBufferMock();

	EXPECT_CALL(bufferMock, clear());
	serializer.clearBuffer();
}

TEST_F(BufferedSerializer_fixture, clearBuffer_clears_access_indexes)
{
	BufferedSerializerTestable serializer;

	auto dummyDataSize = 10u;
	auto& bufferInfo = serializer.getBufferDataInfo();
	bufferInfo.updateAccessIndexesByAddedDataSize(dummyDataSize);
	ASSERT_FALSE(bufferInfo.areAccessIndexesCleared());

	auto& bufferMock = serializer.getBufferMock();
	EXPECT_CALL(bufferMock, clear());

	serializer.clearBuffer();
	EXPECT_TRUE(bufferInfo.areAccessIndexesCleared());
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

	EXPECT_CALL(serializer.getBufferMock(), clear());
	serializer.clear();

	EXPECT_EQ(0, serializer.getReadIndex());
	EXPECT_EQ(0, serializer.getWriteIndex());
}

TEST_F(BufferedSerializer_fixture, Set_indexes_should_return_false_if_file_not_opened)
{
	BufferedSerializerTestable serializer;
	const auto index = 7;

	ASSERT_FALSE(serializer.setReadIndex(index));
	ASSERT_FALSE(serializer.setWriteIndex(index));
}

TEST_F(BufferedSerializer_fixture, Clear_clears_bufferInfo)
{
	auto serializer = makeSerializerWithDefaultDirOpened();

	auto dataSize = 5u;
	auto data = makeByteArray(dataSize);

	EXPECT_CALL(serializer.getBufferMock(), write(A<const ByteArray&>()));
	EXPECT_CALL(serializer.getBufferMock(), clear());
	serializer << data;
	serializer.clear();

	auto& bufferInfo = serializer.getBufferDataInfo();
	EXPECT_TRUE(bufferInfo.isCleared());
}

class TestWriteData_fixture : public BufferedSerializer_fixture
{
public:
	TestWriteData_fixture()
		:DATA_GRATER_THAN_BUFFER(makeByteArray(DATA_SIZE_20)),
		 m_serializer(makeSerializerWithDefaultDirOpened()),
		 m_bufferMock(m_serializer.getBufferMock()),
		 m_fileHandlingMock(m_serializer.getFileHandlingMock()),
		 m_bufferInfo(m_serializer.getBufferDataInfo())
	{

	}

	virtual void SetUp() override
	{

	}

	virtual void TearDown() override
	{
		m_serializer.closeFile();
	}

protected:
	const unsigned int DATA_SIZE_5  = 5u;
	const unsigned int DATA_SIZE_10 = 10u;
	const unsigned int DATA_SIZE_15 = 15u;
	const unsigned int DATA_SIZE_20 = 20u;
	const srl::ByteArray DATA_GRATER_THAN_BUFFER;

	srl::BufferedSerializerTestable m_serializer;
	srl::BufferMock& m_bufferMock;
	srl::FileHandlingMock& m_fileHandlingMock;
	srl::BufferedDataInfo& m_bufferInfo;
};

TEST_F(TestWriteData_fixture, Do_not_buffer_data_if_size_grater_than_buffer)
{
	EXPECT_CALL(m_serializer.getBufferMock(), write(A<const ByteArray&>())).Times(0);
	EXPECT_CALL(m_serializer.getFileHandlingMock(), writeToFile(_, DATA_SIZE_20));

	m_serializer << DATA_GRATER_THAN_BUFFER;
}

TEST_F(TestWriteData_fixture, Access_indexes_should_be_clear_after_write_data_grater_than_buffer)
{
	m_bufferInfo.updateAccessIndexesByAddedDataSize(DATA_SIZE_10);
	ASSERT_FALSE(m_bufferInfo.areAccessIndexesCleared());
	EXPECT_CALL(m_serializer.getFileHandlingMock(), writeToFile(_, DATA_SIZE_20));

	m_serializer << DATA_GRATER_THAN_BUFFER;
	EXPECT_TRUE(m_bufferInfo.areAccessIndexesCleared());
}

TEST_F(TestWriteData_fixture, Buffer_begin_index_should_be_updated_after_write_data_grater_than_buffer)
{
	EXPECT_CALL(m_serializer.getFileHandlingMock(), writeToFile(_, DATA_SIZE_20));

	m_serializer << DATA_GRATER_THAN_BUFFER;
	EXPECT_EQ(DATA_SIZE_20, m_bufferInfo.getBeginIndexRelativelyToFile());
}

TEST_F(TestWriteData_fixture, Write_index_should_be_incremented_after_write_data_grater_than_buffer)
{
	EXPECT_CALL(m_serializer.getFileHandlingMock(), writeToFile(_, DATA_SIZE_20));

	m_serializer << DATA_GRATER_THAN_BUFFER;
	EXPECT_EQ(DATA_SIZE_20, m_serializer.getWriteIndex());
}

TEST_F(TestWriteData_fixture, ByteArray_is_added_to_buffer_if_size_is_lower_than_buffer_size_write_index_zero)
{
	auto byteArray = makeByteArray(DATA_SIZE_10);

	EXPECT_CALL(m_bufferMock, write(byteArray));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);

	m_serializer << byteArray;
}

TEST_F(TestWriteData_fixture, Write_index_should_be_incremented_after_write_of_ByteArray)
{
	auto data = makeByteArray(DATA_SIZE_10);

	EXPECT_CALL(m_bufferMock, write(testing::A<const ByteArray&>()));

	m_serializer << data;

	EXPECT_EQ(data.size(), m_serializer.getWriteIndex());
}

TEST_F(TestWriteData_fixture, Last_correct_indexes_updated_after_write_to_buffer)
{
	auto data = makeByteArray(DATA_SIZE_10);

	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << data;

	EXPECT_NE(0, m_bufferInfo.getLastCorrectWriteIndex());
	EXPECT_NE(0, m_bufferInfo.getLastCorrectReadIndex());
}

TEST_F(TestWriteData_fixture, Data_should_be_added_to_buffer_if_enough_space_available)
{
	auto data = makeByteArray(DATA_SIZE_5);

	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>())).Times(3);
	m_serializer << data << data << data;
}

TEST_F(TestWriteData_fixture, BufferInfo_should_be_updated_after_data_addition)
{
	auto data = makeByteArray(DATA_SIZE_5);

	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _)).Times(0);
	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>())).Times(3);
	m_serializer << data << data << data;

	auto expectedLastWriteIndex = 15u;
	auto expectedLastReadIndex = 14u;
	EXPECT_EQ(expectedLastWriteIndex, m_bufferInfo.getLastCorrectWriteIndex());
	EXPECT_EQ(expectedLastReadIndex, m_bufferInfo.getLastCorrectReadIndex());
}

TEST_F(TestWriteData_fixture, Flush_should_write_data_from_buffer)
{
	auto data = makeByteArray(DATA_SIZE_10);

	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
	m_serializer << data;

	EXPECT_CALL(m_bufferMock, data()).WillOnce(ReturnRef(data));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_10));
	m_serializer.flush();
}

TEST_F(TestWriteData_fixture, Flush_should_clear_access_indexes)
{
	ASSERT_TRUE(m_bufferInfo.areAccessIndexesCleared());
	m_bufferInfo.updateAccessIndexesByAddedDataSize(DATA_SIZE_5);

	auto data = makeByteArray(DATA_SIZE_5);
	EXPECT_CALL(m_bufferMock, data()).WillOnce(ReturnRef(data));
	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, _));

	m_serializer.flush();
	EXPECT_TRUE(m_bufferInfo.areAccessIndexesCleared());
}

//TODO split into two
//implement buffer testable to reduce number of EXPECT_CALLs on buffer

//TEST_F(TestWriteData_fixture, Data_from_buffer_should_be_flushed_if_next_element_exceed_available_space)
//{
//	auto data_1 = makeByteArray(DATA_SIZE_10);
//	auto data_2 = makeByteArray(DATA_SIZE_15);
//
//	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()));
//	m_serializer << data_1;
//
//	EXPECT_CALL(m_bufferMock, data()).WillOnce(ReturnRef(data_1));
//	EXPECT_CALL(m_fileHandlingMock, writeToFile(_, DATA_SIZE_10));
//
//	auto sizeOfTheArrayWrittenToBuffer = 0u;
//	EXPECT_CALL(m_bufferMock, write(A<const ByteArray&>()))
//		.WillOnce(Invoke([&](const auto& arg){sizeOfTheArrayWrittenToBuffer = arg.size();}));
//	
//	m_serializer << data_2;
//	EXPECT_EQ(DATA_SIZE_15, sizeOfTheArrayWrittenToBuffer);
//}

class TestIndexSetters_fixture : public BufferedSerializer_fixture 							 
{
public:
	virtual void SetUp() override
	{
		m_serializer = makeSerializerWithDefaultDirOpened();
	}

	virtual void TearDown() override
	{
		m_serializer.closeFile();
	}

protected:
	const unsigned int FILE_SIZE = SERIALIZER_BUFFER_MIN;
	srl::BufferedSerializerTestable m_serializer;
};

TEST_F(TestIndexSetters_fixture, Set_write_index_should_return_false_if_grater_than_file_size)
{
	EXPECT_FALSE(m_serializer.setWriteIndex(FILE_SIZE + 1));
}

TEST_F(TestIndexSetters_fixture, Set_write_index_should_return_true_if_equal_of_lower_than_file_size)
{
	EXPECT_TRUE(m_serializer.setWriteIndex(FILE_SIZE));
	EXPECT_TRUE(m_serializer.setWriteIndex(FILE_SIZE - 1));
}

TEST_F(TestIndexSetters_fixture, Set_read_index_should_return_false_if_equal_grater_than_file_size)
{
	EXPECT_FALSE(m_serializer.setReadIndex(FILE_SIZE));
	EXPECT_FALSE(m_serializer.setReadIndex(FILE_SIZE + 1));
}

TEST_F(TestIndexSetters_fixture, Set_read_index_should_return_true_if_lower_than_file_size)
{
	EXPECT_TRUE(m_serializer.setReadIndex(FILE_SIZE - 1));
}

TEST_F(TestIndexSetters_fixture, Set_read_write_should_return_false_if_lower_than_zero)
{
	auto negativeValue = -1;

	EXPECT_FALSE(m_serializer.setWriteIndex(negativeValue));
	EXPECT_FALSE(m_serializer.setReadIndex(negativeValue));
}

