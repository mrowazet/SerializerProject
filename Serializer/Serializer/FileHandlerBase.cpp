#include "FileHandlerBase.h"

namespace srl
{

	FileHandlerBase::FileHandlerBase()
	{
	}

	FileHandlerBase::~FileHandlerBase()
	{
	}

	std::unique_ptr<std::fstream> FileHandlerBase::openFileForReadAndWrite(const Path & dir, const IOMode & mode)
	{
		const auto OpenMode = evaluateOpenMode(mode);
		std::unique_ptr<std::fstream> file = std::make_unique<std::fstream>();

		file->open(dir, std::ios_base::binary | std::ios_base::in | std::ios_base::out | OpenMode);

		if (file->fail())
			handleOpenFailed(*file, dir);
			
		return file;
	}

	std::unique_ptr<std::ofstream> FileHandlerBase::openFileForWrite(const Path & dir, const IOMode & mode)
	{
		const auto OpenMode = evaluateOpenMode(mode);
		std::unique_ptr<std::ofstream> file = std::make_unique<std::ofstream>();

		file->open(dir, std::ios_base::binary | std::ios_base::out | OpenMode);

		if (file->fail())
			handleOpenFailed(*file, dir);

		return file;
	}

	std::unique_ptr<std::ifstream> FileHandlerBase::openFileForRead(const Path & dir)
	{
		std::unique_ptr<std::ifstream> file = std::make_unique<std::ifstream>();

		file->open(dir, std::ios_base::binary | std::ios_base::in);

		if (file->fail())
			handleOpenFailed(*file, dir);

		return file;
	}

	void FileHandlerBase::handleOpenFailed(std::ios & file, const Path & dir)
	{
		std::string failureExplanation = "Error: Can not open file: " + dir.string() + "\n";
		throw std::ios_base::failure(failureExplanation);
	}

	std::ios_base::openmode FileHandlerBase::evaluateOpenMode(const IOMode & mode)
	{
		switch (mode)
		{
		case IOMode::Truncate:		return std::ios::trunc;		 break;
		case IOMode::Append:		return std::ios::app;		 break;
		default:					return std::ios::trunc;		 break;
		}
	}

} //end of namespace
