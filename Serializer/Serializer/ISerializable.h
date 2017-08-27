#pragma once
#include <string>

namespace srl
{

	class ISerializable
	{
		friend class Serializer;
	public:
		ISerializable() {};
		virtual ~ISerializable() {};

		ISerializable& asSerializable() { return *this; }
		const ISerializable& asSerializable() const { return *this; };

	protected:
		virtual std::string getAsString() const = 0;
		virtual bool initFromString(const std::string & initString) = 0;
	};

} //end of namespace
