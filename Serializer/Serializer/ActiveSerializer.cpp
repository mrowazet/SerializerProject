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
	if (m_file)
	{
		m_file->close();
		m_file.reset();
		clearFileName();
	}
}

void ActiveSerializer::clearBase()
{
	m_file->close();
	m_file.reset();

	m_file = openFileForReadAndWrite(m_filePath);
}

void ActiveSerializer::clearFileName()
{
	m_filePath.clear();
}

unsigned int ActiveSerializer::size() const
{
	if (m_file)
	{
		return fs::file_size(m_filePath);
	}
	else
	{
		return 0;
	}
}

} //end of namespace
