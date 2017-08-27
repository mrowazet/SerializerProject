#include "Utils.h"
#include "SimpleIO.h"

namespace fs = std::experimental::filesystem;

bool Utils::Tools::createDefaultDirectory()
{
	return fs::create_directory(DEFAULT_DIR);
}

void Utils::Tools::createDefaultOutput()
{
	if (createDefaultDirectory())
		saveToFileAsSerializedString(PARTIAL_TEST_STRING);
}

bool Utils::Tools::createCustomDirectory() 
{
	return fs::create_directory(CUSTOM_DIR);
}

bool Utils::Tools::isDefaultDirectoryCreated()
{
	return isDirectoryCreated(DEFAULT_DIR + DEFAULT_FILE);
}

bool Utils::Tools::isDirectoryCreated(const Path & dir)
{
	Path fileDir = Path(dir);
	return fs::exists(fileDir);
}

void Utils::Tools::cleanUpTestFilesAndDirectories()
{
	setAllFilesInTestDirsAccessible();
	removeAllTestDirs();
}

void Utils::Tools::setAllFilesInTestDirsAccessible()
{
	auto files = getFilesFromAllTestDirs();

	for (auto& file : files)
		giveAllPermsToFile(file);
}

std::vector<Utils::Path> Utils::Tools::getFilesFromAllTestDirs()
{
	std::vector<Path> files;
	
	auto filesInDefaultDir = getFilesInFolder(DEFAULT_DIR);
	auto filesInCustomDir = getFilesInFolder(CUSTOM_DIR);

	files.insert(files.end(), filesInDefaultDir.begin(), filesInDefaultDir.end());
	files.insert(files.end(), filesInCustomDir.begin(), filesInCustomDir.end());

	return files;
}

std::vector<Utils::Path> Utils::Tools::getFilesInFolder(const Path & folder)
{
	std::vector<Path> files;

	if (!fs::exists(folder) || !fs::is_directory(folder))
		return files;

	for (auto& p : fs::directory_iterator(folder))
		files.push_back(p.path());

	return files;
}

void Utils::Tools::giveAllPermsToFile(const Path & file)
{
	fs::permissions(file, fs::perms::all);
}

void Utils::Tools::removeAllTestDirs()
{
	removeFolderIfExist(srl::DEFAULT_OUTPUT_FOLDER);
	removeFolderIfExist(CUSTOM_DIR);
}

void Utils::Tools::removeFolderIfExist(const Path & name)
{
	auto path = Path(name);

	if (fs::exists(path))
		fs::remove_all(path);
}

bool Utils::Tools::setFileReadOnly(const Path & filePath)
{
	if (!isDirectoryCreated(filePath.string()))
		return false;

	fs::permissions(filePath, fs::perms::group_read | fs::perms::others_read);
	return true;
}

Utils::Path Utils::Tools::saveToFileAsSerializedString(const std::string & fileContent, 
													   const std::string & fileName, 
													   const std::string & folderName)
{
	fs::create_directory(folderName);
	auto filePath = fs::path(folderName + fileName);

	std::ofstream file;
	file.open(filePath.string(), std::ios_base::binary | std::ios_base::out | std::ios::trunc);

	const int ContentSize = fileContent.size();
	const char* dataPtr = reinterpret_cast<const char*>(&ContentSize);
	file.write(dataPtr, sizeof(int));

	dataPtr = fileContent.data();
	file.write(dataPtr, fileContent.size());
	file.close();

	return filePath;
}

std::string Utils::Tools::loadBinFileAsString(const Path & dir)
{
	std::ifstream file;
	file.open(dir.string(), std::ios_base::binary | std::ios_base::in);

	if (file.fail())
		return "file.open() failed!";

	auto fileLength = fs::file_size(dir);

	std::string result;;
	result.resize(fileLength);
	file.read(&result[0], fileLength);

	return result;
}

std::string Utils::Tools::getByteArrayAsString(const srl::ByteArray & byteArray)
{
	std::string result;
	int size = byteArray.size();
	const char* character;

	for (int i = 0; i < size; i++)
	{
		character = reinterpret_cast<const char*>(&byteArray[i]);
		result += *character;
	}

	return result;
}