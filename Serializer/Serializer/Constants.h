#pragma once
#include <string>
#include <experimental\filesystem>

namespace srl
{
	using Path = std::experimental::filesystem::path;

	enum class IOMode
	{
		Truncate = 0,
		Append = 1
	};

	const Path DEFAULT_FILE_NAME		= "Serialized.bin";
	const Path DEFAULT_OUTPUT_FOLDER	= "SerializationOutput";
	const Path DEFAULT_DIRECTORY		= DEFAULT_OUTPUT_FOLDER.string() + "\\" + DEFAULT_FILE_NAME.string();

	constexpr unsigned int SERIALIZER_BUFFER_MIN = 16;
	constexpr unsigned int SERIALIZER_BUFFER_MAX = 256;

} //end of namespace