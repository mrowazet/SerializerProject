#include "ActiveSerializer.h"

namespace srl
{

namespace fs = std::experimental::filesystem;

ActiveSerializer::ActiveSerializer()
{

}

ActiveSerializer::ActiveSerializer(ActiveSerializer && serializer)
{
	moveActiveSerializerContent(std::move(serializer));
}

ActiveSerializer & ActiveSerializer::operator=(ActiveSerializer && serializer)
{
	if(this != &serializer)
		moveActiveSerializerContent(std::move(serializer));

	return *this;
}

ActiveSerializer::~ActiveSerializer()
{
	closeFileBase();
}

void ActiveSerializer::moveActiveSerializerContent(ActiveSerializer && serializer)
{
	m_file = std::move(serializer.m_file);
	m_filePath = serializer.m_filePath;

	serializer.clearFileName();
}

bool ActiveSerializer::isFileOpened() const
{
	return 	m_file ? true : false;
}

Path ActiveSerializer::getFilePath() const
{
	return m_filePath;
}

uintmax_t ActiveSerializer::getFileSize() const
{
	return fs::file_size(m_filePath);
}

bool ActiveSerializer::openFileBase(const Path & dir, const IOMode & mode)
{
	if (isFileOpened())
		return false;

	try
	{
		m_file = openFileForReadAndWrite(dir, mode);
		m_filePath = dir;
		return true;
	}
	catch (std::ios_base::failure)
	{
		return false;
	}
}

void ActiveSerializer::closeFileBase()
{
	if (isFileOpened())
	{
		m_file->close();
		m_file.reset();
		clearFileName();
	}
}

void ActiveSerializer::clearBase()
{
	if (isFileOpened())
	{
		m_file->close();
		m_file.reset();
		m_file = openFileForReadAndWrite(m_filePath);
	}
}

void ActiveSerializer::clearFileName()
{
	m_filePath.clear();
}

void ActiveSerializer::ASSERT_FILE_OPENED() const
{
	if (!isFileOpened())
	{
		std::string failureExplanation = "Error: File is not opened!\n";
		throw std::ios_base::failure(failureExplanation);
	}
}

} //end of namespace
