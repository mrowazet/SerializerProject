#include "BufferedDataInfo_fixture.h"

using namespace srl;

BufferedDataInfo_fixture::BufferedDataInfo_fixture()
{
}

BufferedDataInfo_fixture::~BufferedDataInfo_fixture()
{
}

void BufferedDataInfo_fixture::SetUp()
{
}

void BufferedDataInfo_fixture::TearDown()
{
}

TEST_F(BufferedDataInfo_fixture, Default_constructor_set_all_data_to_zero)
{
	BufferedDataInfo info;

	EXPECT_EQ(0, info.getLastCorrectReadIndex());
	EXPECT_EQ(0, info.getLastCorrectWriteIndex());
	EXPECT_EQ(0, info.getBeginIndexRelativelyToFile());
}

TEST_F(BufferedDataInfo_fixture, isClear_return_true_if_all_data_is_zero)
{
	BufferedDataInfo info;

	EXPECT_EQ(0, info.getLastCorrectReadIndex());
	EXPECT_EQ(0, info.getLastCorrectWriteIndex());
	EXPECT_EQ(0, info.getBeginIndexRelativelyToFile());
	EXPECT_TRUE(info.isCleared());
}

TEST_F(BufferedDataInfo_fixture, isCleared_return_false_if_beginIndex_differ_from_zero)
{
	BufferedDataInfo info;

	auto index = 8u;
	info.setBeginIndexRelativelyToFile(index);

	EXPECT_FALSE(info.isCleared());
}

TEST_F(BufferedDataInfo_fixture, isCleared_return_false_if_accessIndexes_differ_from_zero)
{
	BufferedDataInfo info;

	auto dataSize = 6u;
	info.updateAccessIndexesByAddedDataSize(dataSize);

	EXPECT_FALSE(info.isCleared());
}

TEST_F(BufferedDataInfo_fixture, setBeginRelativelyToFile)
{
	BufferedDataInfo info;

	auto index = 8u;
	info.setBeginIndexRelativelyToFile(index);

	EXPECT_EQ(index, info.getBeginIndexRelativelyToFile());
}

TEST_F(BufferedDataInfo_fixture, Clear_all_data_by_clear_function)
{
	BufferedDataInfo info;

	auto index = 4u;
	auto dataSize = 3u;

	info.setBeginIndexRelativelyToFile(index);
	info.updateAccessIndexesByAddedDataSize(dataSize);

	info.clear();
	EXPECT_EQ(0, info.getLastCorrectReadIndex());
	EXPECT_EQ(0, info.getLastCorrectWriteIndex());
	EXPECT_EQ(0, info.getBeginIndexRelativelyToFile());
}

TEST_F(BufferedDataInfo_fixture, Clear_read_write_indexes_only_by_clearAccessIndexes)
{
	BufferedDataInfo info;

	auto index = 5u;
	auto dataSize = 8u;

	info.setBeginIndexRelativelyToFile(index);
	info.updateAccessIndexesByAddedDataSize(dataSize);

	info.clearAccessIndexes();
	EXPECT_EQ(0, info.getLastCorrectReadIndex());
	EXPECT_EQ(0, info.getLastCorrectWriteIndex());
	EXPECT_EQ(index, info.getBeginIndexRelativelyToFile());
}

TEST_F(BufferedDataInfo_fixture, Update_set_read_write_indexes_to_proper_value_if_indexes_cleared)
{
	BufferedDataInfo info;
	auto dataSize = 5;

	info.updateAccessIndexesByAddedDataSize(dataSize);

	EXPECT_EQ(dataSize, info.getLastCorrectWriteIndex());
	EXPECT_EQ(dataSize - 1, info.getLastCorrectReadIndex());
}

TEST_F(BufferedDataInfo_fixture, Update_set_read_write_indexes_to_proper_value_if_indexes_are_not_zeroes)
{
	BufferedDataInfo info;
	auto dataSize = 5;

	info.updateAccessIndexesByAddedDataSize(dataSize);
	info.updateAccessIndexesByAddedDataSize(dataSize);

	auto expectedWriteIndex = 2 * dataSize;
	auto expectedReadIndex = expectedWriteIndex - 1;

	EXPECT_EQ(expectedWriteIndex, info.getLastCorrectWriteIndex());
	EXPECT_EQ(expectedReadIndex, info.getLastCorrectReadIndex());
}

