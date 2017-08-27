#pragma once
#include <fstream>
#include <memory>
#include "IFileHandler.h"
#include "Constants.h"
#include "Utilities.h"
#include "Serializer.h"

namespace srl
{

class FileHandlerBase : public IFileHandler
{
public:
	FileHandlerBase();
	virtual ~FileHandlerBase();

	static std::unique_ptr<std::fstream> openFileForReadAndWrite(const Path & dir, const IOMode & mode = IOMode::Truncate);
	static std::unique_ptr<std::ofstream> openFileForWrite(const Path & dir, const IOMode & mode = IOMode::Truncate);
	static std::unique_ptr<std::ifstream> openFileForRead(const Path & dir);

protected:
	static void handleOpenFailed(std::ios & file, const Path & dir);
	static std::ios_base::openmode evaluateOpenMode(const IOMode & mode);

};

} //end of namespace

