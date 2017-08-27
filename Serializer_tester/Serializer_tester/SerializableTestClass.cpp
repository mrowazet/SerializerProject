#include "SerializableTestClass.h"


SerializableTestClass::SerializableTestClass()
	:m_testString("Default_test_string")
{

}

SerializableTestClass::SerializableTestClass(const std::string & testString)
	:m_testString(testString)
{

}

SerializableTestClass::~SerializableTestClass()
{
}

std::string SerializableTestClass::getAsString() const
{
	return m_testString;
}

bool SerializableTestClass::initFromString(const std::string & initString)
{
	return false;
}
