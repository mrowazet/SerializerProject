#pragma once
#include <fstream>
#include "ISerializer.h"
#include "FileHandlerBase.h"

namespace srl
{

class ActiveSerializer : public ISerializer, protected FileHandlerBase
{
public:
	ActiveSerializer();
	virtual ~ActiveSerializer();

	ActiveSerializer(ActiveSerializer && serializer);
	ActiveSerializer & operator=(ActiveSerializer && serializer);

	ActiveSerializer(const ActiveSerializer & serializer) = delete;
	ActiveSerializer & operator=(const ActiveSerializer & serializer) = delete;

	virtual bool openFile(const Path & dir, const IOMode & mode = IOMode::Truncate) = 0;
	virtual void closeFile() = 0;

	virtual bool isFileOpened() const;
	virtual Path getFilePath() const;
	virtual uintmax_t getFileSize() const;

protected:
	std::unique_ptr<std::fstream> m_file;
	Path m_filePath;

	void moveActiveSerializerContent(ActiveSerializer && serializer);

	bool openFileBase(const Path & dir, const IOMode & mode = IOMode::Truncate);
	void closeFileBase();
	void clearBase();

	virtual void writeToFile(const char* data, const unsigned int size) = 0;
	virtual void readFromFile(const unsigned int size) const = 0;

	void clearFileName();

	void ASSERT_FILE_OPENED() const;
};

} //end of namespace
