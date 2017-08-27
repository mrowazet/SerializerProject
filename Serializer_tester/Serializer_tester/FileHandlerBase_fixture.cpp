#include "FileHandlerBase_fixture.h"

using namespace srl;

FileHandlerBase_fixture::FileHandlerBase_fixture()
{
}

FileHandlerBase_fixture::~FileHandlerBase_fixture()
{
}

void FileHandlerBase_fixture::SetUp()
{
	
}

void FileHandlerBase_fixture::TearDown()
{
	if (m_inFile)
		m_inFile->close();

	if (m_outFile)
		m_outFile->close();

	if (m_inOutFile)
		m_inOutFile->close();

	ut::cleanUpTestFilesAndDirectories();
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_trucnate_mode)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_outFile = FileHandlerBase::openFileForWrite(DEFAULT_DIRECTORY, srl::IOMode::Truncate));
	EXPECT_TRUE(m_outFile->is_open());

	auto fileSize = fs::file_size(DEFAULT_DIRECTORY);
	EXPECT_EQ(0, fileSize);
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_append_mode)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_outFile = FileHandlerBase::openFileForWrite(DEFAULT_DIRECTORY, srl::IOMode::Append));
	EXPECT_TRUE(m_outFile->is_open());

	auto expectedFileSize = sizeof(int) + PARTIAL_TEST_STRING.size();
	auto fileSize = fs::file_size(DEFAULT_DIRECTORY);
	EXPECT_EQ(expectedFileSize, fileSize);
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_directory_does_not_exist)
{
	ASSERT_THROW(FileHandlerBase::openFileForWrite(DEFAULT_DIRECTORY), 
				 std::ios_base::failure);
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_read_only_file)
{
	ut::createDefaultOutput();
	ut::setFileReadOnly(DEFAULT_DIRECTORY);

	ASSERT_THROW(m_outFile = FileHandlerBase::openFileForWrite(DEFAULT_DIRECTORY, srl::IOMode::Truncate),
				 std::ios_base::failure);
}

TEST_F(FileHandlerBase_fixture, open_file_for_read)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_inFile = FileHandlerBase::openFileForRead(DEFAULT_DIRECTORY));
	EXPECT_TRUE(m_inFile->is_open());
}

TEST_F(FileHandlerBase_fixture, open_file_for_read_directory_does_not_exist)
{
	ASSERT_THROW(FileHandlerBase::openFileForRead(DEFAULT_DIRECTORY),
				 std::ios_base::failure);
}

TEST_F(FileHandlerBase_fixture, open_file_for_read_and_write)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_inOutFile = FileHandlerBase::openFileForReadAndWrite(DEFAULT_DIRECTORY));
	EXPECT_TRUE(m_inOutFile->is_open());
}

TEST_F(FileHandlerBase_fixture, open_file_for_read_and_write_but_file_is_read_only)
{ 
	ut::createDefaultOutput();
	ut::setFileReadOnly(DEFAULT_DIRECTORY);

	ASSERT_THROW(m_inOutFile = FileHandlerBase::openFileForReadAndWrite(DEFAULT_DIRECTORY),
			     std::ios_base::failure);
}

TEST_F(FileHandlerBase_fixture, open_file_for_read_and_write_directory_does_not_exist)
{
	ASSERT_THROW(m_inOutFile = FileHandlerBase::openFileForReadAndWrite(DEFAULT_DIRECTORY),
				 std::ios_base::failure);
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_and_read_in_truncate_mode)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_inOutFile = FileHandlerBase::openFileForReadAndWrite(DEFAULT_DIRECTORY, srl::IOMode::Truncate));
	EXPECT_TRUE(m_inOutFile->is_open());

	auto fileSize = fs::file_size(DEFAULT_DIRECTORY);
	EXPECT_EQ(0, fileSize);
}

TEST_F(FileHandlerBase_fixture, open_file_for_write_and_read_in_append_mode)
{
	ut::createDefaultOutput();

	ASSERT_NO_THROW(m_inOutFile = FileHandlerBase::openFileForReadAndWrite(DEFAULT_DIRECTORY, srl::IOMode::Append));
	EXPECT_TRUE(m_inOutFile->is_open());

	auto expectedFileSize = sizeof(int) + PARTIAL_TEST_STRING.size();
	auto fileSize = fs::file_size(DEFAULT_DIRECTORY);
	EXPECT_EQ(expectedFileSize, fileSize);
}