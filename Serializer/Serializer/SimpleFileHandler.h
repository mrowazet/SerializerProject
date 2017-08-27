#pragma once
#include "FileHandlerBase.h"

namespace srl
{

class SimpleFileHandler : public FileHandlerBase
{

public:
	static bool saveToBinFile(const Serializer & serializer,
							  const IOMode mode);

	static bool saveToBinFile(const Serializer & serializer,
							  const Path & dir = DEFAULT_DIRECTORY,
							  const IOMode mode = IOMode::Truncate);

	static bool loadFromBinFile(Serializer & serializer,
							    const IOMode mode);

	static bool loadFromBinFile(Serializer & serializer,
								const Path & dir = DEFAULT_DIRECTORY,
								const IOMode mode = IOMode::Truncate);

private:
	static void writeDataToFile(const ByteArray & data, std::unique_ptr<std::ofstream> & fileStream);
	static void loadDataInTruncateMode(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir);
	static void loadDataInAppendMode(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir);
	static void loadDataFromFile(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir);
	static std::pair<bool, int> isWriteIndexOnLastPositon(const Serializer & serializer);
};

} //end of namespace

