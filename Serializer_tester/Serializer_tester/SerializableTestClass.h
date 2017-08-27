#pragma once
#include "ISerializable.h"
#include <string>

class SerializableTestClass : public srl::ISerializable
{
	public:
		SerializableTestClass();
		SerializableTestClass(const std::string & testString);
		virtual ~SerializableTestClass();

	protected:
		virtual std::string getAsString() const override;
		virtual bool initFromString(const std::string & initString) override;

	private:
		const std::string m_testString;
};

