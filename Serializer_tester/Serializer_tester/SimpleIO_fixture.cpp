#include "SimpleIO_fixture.h"

using namespace srl;
using namespace Utils;

SimpleIO_fixture::TestData::TestData()
{
	intValues = { 1, 2, 3, 4, 5 };
	charValues = { '1', '2', '3', '4', '5' };
	doubleValues = { 54323.3 };
	floatValues = { 332.32f };
	stringValue = PARTIAL_TEST_STRING;
	c_strValue = " c_str ";
}

SimpleIO_fixture::SimpleIO_fixture()
	:SERIALIZER(makeByteArrayForTest())
{

}

SimpleIO_fixture::~SimpleIO_fixture()
{

}

void SimpleIO_fixture::SetUp()
{

}

void SimpleIO_fixture::TearDown()
{
	Tools::cleanUpTestFilesAndDirectories();
}

srl::ByteArray SimpleIO_fixture::makeByteArrayForTest()
{
	Serializer testDataSerializer;
	
	for (auto& value : testData.intValues)
		testDataSerializer << value;

	for (auto& value : testData.charValues)
		testDataSerializer << value;

	testDataSerializer << PARTIAL_TEST_STRING;

	for (auto& value : testData.doubleValues)
		testDataSerializer << value;

	testDataSerializer << testData.c_strValue.c_str();

	for (auto& value : testData.floatValues)
		testDataSerializer << value;
	
	return testDataSerializer.getData();
}

TEST_F(SimpleIO_fixture, SERIALIZER_filled_correctly)
{
	ByteArray byteArray = makeByteArrayForTest(); 
	std::string expected = Tools::getByteArrayAsString(byteArray);

	auto result = Tools::getByteArrayAsString(SERIALIZER.getData());
	ASSERT_EQ(expected, result);
}

TEST_F(SimpleIO_fixture, default_directory_created_when_using_2_params_func_for_save)
{
	EXPECT_TRUE(SimpleIO::saveToBinFile(SERIALIZER, IOMode::Append));

	EXPECT_TRUE(Tools::isDirectoryCreated(DEFAULT_OUTPUT_FOLDER)) << "Folder not created!\n";
	EXPECT_TRUE(Tools::isDirectoryCreated(DEFAULT_OUTPUT_FOLDER.string() + "\\" + DEFAULT_FILE)) << "File not create!";
}

TEST_F(SimpleIO_fixture, default_directory_created_when_using_1_param_only)
{
	EXPECT_TRUE(SimpleIO::saveToBinFile(SERIALIZER));

	EXPECT_TRUE(Tools::isDirectoryCreated(DEFAULT_OUTPUT_FOLDER)) << "Folder not created!\n";
	EXPECT_TRUE(Tools::isDirectoryCreated(DEFAULT_OUTPUT_FOLDER.string() + "\\" + DEFAULT_FILE)) << "File not create!";
}

TEST_F(SimpleIO_fixture, file_created_in_custom_correct_directory)
{
	Tools::createCustomDirectory();

	EXPECT_TRUE(SimpleIO::saveToBinFile(SERIALIZER, CUSTOM_DIR + CUSTOM_FILE));

	EXPECT_TRUE(Tools::isDirectoryCreated(CUSTOM_DIR)) << "Folder not created!\n";
	EXPECT_TRUE(Tools::isDirectoryCreated(CUSTOM_DIR + CUSTOM_FILE)) << "File not create!";
}

TEST_F(SimpleIO_fixture, file_not_created_when_custom_directory_is_not_created_throws_exception)
{
	EXPECT_THROW(SimpleIO::saveToBinFile(SERIALIZER, INCORRECT_CUSTOM_DIR), std::ios_base::failure);
	EXPECT_FALSE(Tools::isDirectoryCreated(INCORRECT_CUSTOM_DIR));
}

TEST_F(SimpleIO_fixture, data_correctly_stored_in_file)
{
	SimpleIO::saveToBinFile(SERIALIZER);
	ASSERT_TRUE(Tools::isDefaultDirectoryCreated());

	auto stringFromByteArray = Tools::getByteArrayAsString(SERIALIZER.getData());
	auto stringFromFile = Tools::loadBinFileAsString(DEFAULT_DIRECTORY);
	EXPECT_EQ(stringFromByteArray, stringFromFile);
}

TEST_F(SimpleIO_fixture, write_to_file_in_app_mode)
{
	SimpleIO::saveToBinFile(SERIALIZER);
	ASSERT_TRUE(Tools::isDefaultDirectoryCreated());
	auto fileLengthAfterFirstWrite = fs::file_size(DEFAULT_DIR + DEFAULT_FILE);

	SimpleIO::saveToBinFile(SERIALIZER, IOMode::Append);
	auto fileLengthAfterSecondWrite = fs::file_size(DEFAULT_DIR + DEFAULT_FILE);

	auto expectedFileLength = 2 * fileLengthAfterFirstWrite;
	EXPECT_EQ(expectedFileLength, fileLengthAfterSecondWrite);

	auto expectedString = Tools::getByteArrayAsString(SERIALIZER.getData());
	expectedString += expectedString;

	auto stringFromFile = Tools::loadBinFileAsString(DEFAULT_DIRECTORY);
	EXPECT_EQ(expectedString, stringFromFile);
}

TEST_F(SimpleIO_fixture, write_to_file_in_trunc_mode)
{
	SimpleIO::saveToBinFile(SERIALIZER);
	ASSERT_TRUE(Tools::isDefaultDirectoryCreated());
	auto fileLengthAfterFirstWrite = fs::file_size(DEFAULT_DIR + DEFAULT_FILE);

	SimpleIO::saveToBinFile(SERIALIZER, IOMode::Truncate);
	auto fileLengthAfterSecondWrite = fs::file_size(DEFAULT_DIR + DEFAULT_FILE);

	EXPECT_EQ(fileLengthAfterFirstWrite, fileLengthAfterSecondWrite);

	auto expectedString = Tools::getByteArrayAsString(SERIALIZER.getData());

	auto stringFromFile = Tools::loadBinFileAsString(DEFAULT_DIRECTORY);
	EXPECT_EQ(expectedString, stringFromFile);
}

TEST_F(SimpleIO_fixture, data_stored_in_rigth_order_in_append_mode)
{
	SimpleIO::saveToBinFile(SERIALIZER);
	ASSERT_TRUE(Tools::isDefaultDirectoryCreated());

	Serializer additionalData;
	additionalData << 'c' << 'h' << 'a' << 'r';

	SimpleIO::saveToBinFile(additionalData, IOMode::Append);

	auto expectedString = Tools::getByteArrayAsString(SERIALIZER.getData());
	expectedString += "char";

	auto stringFromFile = Tools::loadBinFileAsString(DEFAULT_DIRECTORY);
	EXPECT_EQ(expectedString, stringFromFile);
}

TEST_F(SimpleIO_fixture, write_to_file_permission_denied_throws_exception)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	ASSERT_TRUE(Tools::setFileReadOnly(filePath));

	EXPECT_THROW(SimpleIO::saveToBinFile(SERIALIZER, filePath.string()), std::ios_base::failure);
}

TEST_F(SimpleIO_fixture, open_file_from_default_directory)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);

	Serializer serializer;
	ASSERT_TRUE(srl::SimpleIO::loadFromBinFile(serializer));
}

TEST_F(SimpleIO_fixture, open_file_from_default_direcotry_when_it_is_not_created)
{
	Serializer serializer;
	EXPECT_THROW(SimpleIO::loadFromBinFile(serializer), std::ios_base::failure);
}

TEST_F(SimpleIO_fixture, open_file_from_default_direcotry_when_file_does_not_exist)
{
	fs::create_directory(DEFAULT_DIR);
	fs::path filePath(DEFAULT_DIR + DEFAULT_FILE);

	Serializer serializer;
	EXPECT_THROW(SimpleIO::loadFromBinFile(serializer, filePath.string()), std::ios_base::failure);
}

TEST_F(SimpleIO_fixture, open_file_from_custom_directory)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING, CUSTOM_FILE, CUSTOM_DIR);

	Serializer serializer;
	ASSERT_TRUE(srl::SimpleIO::loadFromBinFile(serializer, filePath.string()));
}

TEST_F(SimpleIO_fixture, open_file_from_custom_direcotry_when_it_is_not_created)
{
	fs::path filePath(INCORRECT_CUSTOM_DIR + CUSTOM_FILE);

	Serializer serializer;
	EXPECT_THROW(SimpleIO::loadFromBinFile(serializer, filePath.string()), std::ios_base::failure);
}

TEST_F(SimpleIO_fixture, open_file_from_custom_direcotry_when_file_does_not_exist)
{
	fs::create_directory(CUSTOM_DIR);
	fs::path filePath(CUSTOM_DIR + CUSTOM_FILE);

	Serializer serializer;
	EXPECT_THROW(SimpleIO::loadFromBinFile(serializer, filePath.string()), std::ios_base::failure);
}

TEST_F(SimpleIO_fixture, data_correctly_loaded_from_file)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	auto expectedData = Tools::loadBinFileAsString(filePath);

	Serializer serializer;
	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer));

	auto loadedData = Tools::getByteArrayAsString(serializer.getData());
	EXPECT_EQ(expectedData, loadedData);
}

TEST_F(SimpleIO_fixture, get_various_data_from_loaded_file)
{
	SimpleIO::saveToBinFile(SERIALIZER);

	Serializer serializer;
	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer));

	//Check loaded data
	for (auto& value : testData.intValues)
	{
		int loadedValue;
		serializer >> loadedValue;
		EXPECT_EQ(value, loadedValue);
	}

	for (auto& value : testData.charValues)
	{
		char loadedValue;
		serializer >> loadedValue;
		EXPECT_EQ(value, loadedValue);
	}

	std::string loadedString;
	serializer >> loadedString;
	EXPECT_EQ(PARTIAL_TEST_STRING, loadedString);

	for (auto& value : testData.doubleValues)
	{
		double loadedValue;
		serializer >> loadedValue;
		EXPECT_EQ(value, loadedValue);
	}

	serializer >> loadedString;
	EXPECT_EQ(testData.c_strValue.c_str(), loadedString);

	for (auto& value : testData.floatValues)
	{
		float loadedValue;
		serializer >> loadedValue;
		EXPECT_EQ(value, loadedValue);
	}
}

/*
TEST_F(SimpleIO_fixture, read_from_file_permission_denied)
{
	auto filePath = saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	setFileReadOnly(filePath);

	//On Windows permissions must be changed manually!
	//It is not possible to remove all bits via std::filesystem::permissions
	//Only "ReadOnly" file can be achived that way...

	Serializer serializer;
	EXPECT_THROW(SimpleIO::loadFromBinFile(serializer, filePath.string()), std::ios_base::failure);
}
*/

TEST_F(SimpleIO_fixture, read_from_file_using_2_params_function)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	auto expectedData = Tools::loadBinFileAsString(filePath);

	Serializer serializer;
	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer, IOMode::Truncate));

	auto loadedData = Tools::getByteArrayAsString(serializer.getData());
	EXPECT_EQ(expectedData, loadedData);
}

TEST_F(SimpleIO_fixture, read_from_file_in_append_mode)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);

	const std::string AdditionalText = "AdditionalTestString";

	Serializer serializer;
	serializer << AdditionalText;

	auto storedData = Tools::getByteArrayAsString(serializer.getData());
	auto expectedData = storedData + Tools::loadBinFileAsString(filePath);

	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer, IOMode::Append));

	auto data = Tools::getByteArrayAsString(serializer.getData());
	EXPECT_EQ(expectedData, data);
}

TEST_F(SimpleIO_fixture, read_from_file_in_truncate_mode)
{
	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);

	const std::string AdditionalText = "AdditionalTestString";

	Serializer serializer;
	serializer << AdditionalText;

	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer, IOMode::Truncate));

	auto expectedData = Tools::loadBinFileAsString(filePath);
	auto data = Tools::getByteArrayAsString(serializer.getData());
	EXPECT_EQ(expectedData, data);
}


TEST_F(SimpleIO_fixture, writeIndex_should_be_moved_to_last_position)
{
	//Precondition: serializer not empty
	//writeIndex on last position
	//Action: load additional data from file in Append mode
	//Result: writeIndex on last positon 

	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	const std::string AdditionalText = "AdditionalTestString";

	Serializer serializer;
	serializer << AdditionalText;
	ASSERT_EQ(serializer.size(), serializer.getWriteIndex());

	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer, IOMode::Append));
	EXPECT_EQ(serializer.size(), serializer.getWriteIndex());
}

TEST_F(SimpleIO_fixture, writeIndex_should_stay_in_current_position)
{
	//Precondition: serializer not empty
	//writeIndex on position != last
	//Action: load additional data from file in Append mode
	//Result: writeIndex stayed on curren postion

	auto filePath = Tools::saveToFileAsSerializedString(PARTIAL_TEST_STRING);
	const std::string AdditionalText = "AdditionalTestString";

	Serializer serializer;
	serializer << AdditionalText;
	ASSERT_EQ(serializer.size(), serializer.getWriteIndex());

	const int customWriteIndex = 8;
	serializer.setWriteIndex(customWriteIndex);

	EXPECT_TRUE(SimpleIO::loadFromBinFile(serializer, IOMode::Append));
	EXPECT_EQ(customWriteIndex, serializer.getWriteIndex());
}
