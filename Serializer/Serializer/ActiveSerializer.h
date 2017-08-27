#pragma once
#include <fstream>
#include "ISerializer.h"
#include "FileHandlerBase.h"

namespace srl
{

class ActiveSerializer : public ISerializer, public FileHandlerBase
{
public:
	ActiveSerializer();
	virtual ~ActiveSerializer();

	ActiveSerializer(ActiveSerializer && serializer);
	ActiveSerializer & operator=(ActiveSerializer && serializer);

	ActiveSerializer(const ActiveSerializer & serializer) = delete;
	ActiveSerializer & operator=(const ActiveSerializer & serializer) = delete;

	virtual unsigned int size() const override;

	virtual bool openFile(const Path & dir, const IOMode & mode = IOMode::Truncate) = 0;
	virtual void closeFile() = 0;

	bool isFileOpened() const;
	Path getFilePath() const;
	uintmax_t getFileSize() const;

protected:
	std::unique_ptr<std::fstream> m_file;
	Path m_filePath;

	void moveActiveSerializerContent(ActiveSerializer && serializer);

	bool openFileBase(const Path & dir, const IOMode & mode = IOMode::Truncate);
	void closeFileBase();
	void clearBase();

	void clearFileName();
};

} //end of namespace
