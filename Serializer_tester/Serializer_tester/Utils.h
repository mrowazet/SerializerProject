#pragma once
#include "TestConstants.h"
#include "Serializer.h"
#include <experimental\filesystem>

namespace Utils { class Tools; };
using ut = Utils::Tools;
namespace fs = std::experimental::filesystem;

namespace Utils
{
	using Path = std::experimental::filesystem::path;

class Tools
{
	public:
		static bool createDefaultDirectory();
		static void createDefaultOutput();
		static bool createCustomDirectory();
		static bool isDefaultDirectoryCreated();
		static bool isDirectoryCreated(const Path & dir);

		static void cleanUpTestFilesAndDirectories();	

		static bool setFileReadOnly(const Path & filePath);
		static void giveAllPermsToFile(const Path & file);

		static Path saveToFileAsSerializedString(const std::string & fileContent,
												 const std::string & fileName = DEFAULT_FILE,
												 const std::string & folderName = DEFAULT_DIR);
		static std::string loadBinFileAsString(const Path & dir);

		static std::string getByteArrayAsString(const srl::ByteArray & byteArray);

	private:
		static void setAllFilesInTestDirsAccessible();
		static std::vector<Path> getFilesFromAllTestDirs();
		static std::vector<Path> getFilesInFolder(const Path & folder);
		static void removeAllTestDirs();
		static void removeFolderIfExist(const Path & name);
};

} //end of namespace