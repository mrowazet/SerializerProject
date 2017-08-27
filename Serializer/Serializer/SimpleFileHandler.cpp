#include "SimpleIO.h"

namespace srl
{
	namespace fs = std::experimental::filesystem;

	bool SimpleFileHandler::saveToBinFile(const Serializer & serializer, const IOMode mode)
	{
		return saveToBinFile(serializer, DEFAULT_DIRECTORY, mode);
	}

	bool SimpleFileHandler::saveToBinFile(const Serializer & serializer, const Path & dir, const IOMode mode)
	{
		if (Utilities::isDefaultDirectory(dir) && !fs::exists(DEFAULT_OUTPUT_FOLDER))
			fs::create_directory(DEFAULT_OUTPUT_FOLDER);

		auto file = openFileForWrite(dir, mode);

		if (file->is_open())
		{
			writeDataToFile(serializer.getData(), file);
			file->close();

			return true;
		}
		else
		{
			return false;
		}
	}

	void SimpleFileHandler::writeDataToFile(const ByteArray & data, std::unique_ptr<std::ofstream> & fileStream)
	{
		const auto Size = data.size();
		const char* dataPtr = reinterpret_cast<const char*>(&data[0]);

		fileStream->write(dataPtr, Size);
	}

	bool SimpleFileHandler::loadFromBinFile(Serializer & serializer, const IOMode mode)
	{
		return loadFromBinFile(serializer, DEFAULT_DIRECTORY, mode);
	}

	bool SimpleFileHandler::loadFromBinFile(Serializer & serializer, const Path & dir, const IOMode mode)
	{
		auto file = openFileForRead(dir);

		if (file->is_open())
		{
			switch (mode)
			{
			case IOMode::Truncate:	loadDataInTruncateMode(serializer, file, dir);		break;
			case IOMode::Append:	loadDataInAppendMode(serializer, file, dir);		break;
			}

			file->close();
			return true;
		}
		else
		{
			return false;
		}
	}

	void SimpleFileHandler::loadDataInTruncateMode(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir)
	{
		serializer.clear();
		loadDataFromFile(serializer, file, dir);
	}

	void SimpleFileHandler::loadDataFromFile(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir)
	{
		auto initialBufferSize = serializer.size();
		auto dataLength = fs::file_size(dir);

		auto& internalBuffer = serializer.m_data;
		internalBuffer.resize(initialBufferSize + dataLength);
		serializer.setWriteIndex(internalBuffer.size());

		char* newStorageBegin = reinterpret_cast<char*>(&internalBuffer[initialBufferSize]);
		file->read(newStorageBegin, dataLength);
	}

	void SimpleFileHandler::loadDataInAppendMode(Serializer & serializer, std::unique_ptr<std::ifstream> & file, const Path & dir)
	{
		auto writeIndexStatus = isWriteIndexOnLastPositon(serializer);
		auto indexOnLastPos = writeIndexStatus.first;
		auto initialWriteIndex = writeIndexStatus.second;

		if (!indexOnLastPos)
			serializer.setWriteIndex(serializer.size());

		loadDataFromFile(serializer, file, dir);

		if (!indexOnLastPos)
			serializer.setWriteIndex(initialWriteIndex);
	}

	std::pair<bool, int> SimpleFileHandler::isWriteIndexOnLastPositon(const Serializer & serializer)
	{
		std::pair<bool, int> result;
		result.first = true;
		result.second = serializer.getWriteIndex();

		if (result.second != serializer.size())
		{
			result.first = false;
		}

		return result;
	}

} //end of namespace