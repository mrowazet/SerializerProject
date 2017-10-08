#include "ActiveSerializer_fixture.h"

using namespace testing;
using namespace srl;

ActiveSerializer_fixture::ActiveSerializer_fixture()
{
}

ActiveSerializer_fixture::~ActiveSerializer_fixture()
{
}

void ActiveSerializer_fixture::SetUp()
{

}

void ActiveSerializer_fixture::TearDown()
{
	ut::cleanUpTestFilesAndDirectories();
}

TEST_F(ActiveSerializer_fixture, ActiveSerializerTestable_can_be_instantiated)
{
	ActiveSerializerTestable serializer;
}

TEST_F(ActiveSerializer_fixture, isFileOpened_returns_true_if_file_opened)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	EXPECT_TRUE(serializer.isFileOpened());
}

TEST_F(ActiveSerializer_fixture, isFileOpened_returns_false_after_close)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_TRUE(serializer.isFileOpened());

	serializer.closeFile();
	EXPECT_FALSE(serializer.isFileOpened());
}

TEST_F(ActiveSerializer_fixture, getFilePath_returns_epmty_path_if_file_is_not_opened)
{
	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_FALSE(serializer.isFileOpened());

	auto filePath = serializer.getFilePath();
	EXPECT_EQ("", filePath.string());
}

TEST_F(ActiveSerializer_fixture, getFilePath_return_full_file_path_if_file_is_opened)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_TRUE(serializer.isFileOpened());

	auto filePath = serializer.getFilePath();
	EXPECT_EQ(DEFAULT_DIRECTORY.string(), filePath.string());
}

TEST_F(ActiveSerializer_fixture, getFilePath_returns_empty_path_after_close)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_TRUE(serializer.isFileOpened());
	serializer.closeFile();

	auto filePath = serializer.getFilePath();
	EXPECT_EQ("", filePath.string());
}

TEST_F(ActiveSerializer_fixture, getFileSize_throws_exception_if_file_not_opened)
{
	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_FALSE(serializer.isFileOpened());

	ASSERT_THROW(serializer.getFileSize(), std::exception);
}

TEST_F(ActiveSerializer_fixture, size_returns_zero_if_file_not_opened)
{
	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_FALSE(serializer.isFileOpened());

	const int SIZE_ZERO = 0;
	const int FileSize = serializer.size();
	EXPECT_EQ(SIZE_ZERO, FileSize);
}

TEST_F(ActiveSerializer_fixture, getFileSize_return_size_of_the_opened_file)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	auto expectedFileSize = sizeof(int) + PARTIAL_TEST_STRING.size();
	auto fileSize = serializer.getFileSize();
	EXPECT_EQ(expectedFileSize, fileSize);
}

TEST_F(ActiveSerializer_fixture, size_returns_zero_after_close)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	ASSERT_TRUE(serializer.isFileOpened());
	serializer.closeFile();

	const int SIZE_ZERO = 0;
	EXPECT_EQ(SIZE_ZERO, serializer.size());
}

TEST_F(ActiveSerializer_fixture, directory_does_not_exist_isFileOpened_returns_false)
{
	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	EXPECT_FALSE(serializer.isFileOpened());
}

TEST_F(ActiveSerializer_fixture, directory_exist_isFileOpened_returns_true)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	EXPECT_TRUE(serializer.isFileOpened());
}

TEST_F(ActiveSerializer_fixture, new_file_created_if_folder_exists)
{
	ut::createDefaultDirectory();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY);
	EXPECT_TRUE(ut::isDirectoryCreated(DEFAULT_DIRECTORY));
}

TEST_F(ActiveSerializer_fixture, file_cleared_if_opened_in_truncate_mode)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Truncate);
	ASSERT_TRUE(serializer.isFileOpened());

	const int EMPTY_FILE = 0u;
	EXPECT_EQ(EMPTY_FILE, serializer.getFileSize());
}

TEST_F(ActiveSerializer_fixture, file_not_cleared_if_opened_in_append_mode)
{
	ut::createDefaultOutput();
	const auto FileSize = fs::file_size(DEFAULT_DIRECTORY);

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	EXPECT_EQ(FileSize, serializer.getFileSize());
}

TEST_F(ActiveSerializer_fixture, clear_clears_whole_file_content)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());
	ASSERT_GT(serializer.getFileSize(), 0);

	serializer.clear();
	EXPECT_EQ(0, serializer.getFileSize());
}

TEST_F(ActiveSerializer_fixture, file_path_is_correct_after_clear)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	auto filePathBeforeClear = serializer.getFilePath();

	serializer.clear();
	auto filePathAfterClear = serializer.getFilePath();

	EXPECT_EQ(filePathBeforeClear, filePathAfterClear);
}

TEST_F(ActiveSerializer_fixture, ActiveSerializer_can_be_moved_by_constructor)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	ActiveSerializerTestable destSerializer(std::move(serializer));

	ASSERT_TRUE(destSerializer.isFileOpened());
	ASSERT_EQ(DEFAULT_DIRECTORY, destSerializer.getFilePath());
	EXPECT_GT(destSerializer.getFileSize(), 0);

	EXPECT_FALSE(serializer.isFileOpened());
	EXPECT_EQ("", serializer.getFilePath());
	EXPECT_THROW(serializer.getFileSize(), std::exception);
}

TEST_F(ActiveSerializer_fixture, ActiveSerializer_can_be_moved_by_assignment_operator)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	ActiveSerializerTestable destSerializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	destSerializer = std::move(serializer);

	ASSERT_TRUE(destSerializer.isFileOpened());
	ASSERT_EQ(DEFAULT_DIRECTORY, destSerializer.getFilePath());
	EXPECT_GT(destSerializer.getFileSize(), 0);

	EXPECT_FALSE(serializer.isFileOpened());
	EXPECT_EQ("", serializer.getFilePath());
	EXPECT_THROW(serializer.getFileSize(), std::exception);
}

TEST_F(ActiveSerializer_fixture, clear_is_safe_when_no_file_opened)
{
	ActiveSerializerTestable serializer;
	EXPECT_NO_THROW(serializer.clear());
}

TEST_F(ActiveSerializer_fixture, open_returns_false_if_file_currently_opened)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	EXPECT_FALSE(serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append));
}

TEST_F(ActiveSerializer_fixture, file_can_be_opened_again_after_close)
{
	ut::createDefaultOutput();

	ActiveSerializerTestable serializer;
	serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append);
	ASSERT_TRUE(serializer.isFileOpened());

	serializer.closeFile();
	EXPECT_TRUE(serializer.openFile(DEFAULT_DIRECTORY, IOMode::Append));
}