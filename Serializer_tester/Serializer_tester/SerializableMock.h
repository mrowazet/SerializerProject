#pragma once
#include "ISerializable.h"
#include "gtest\gtest.h"
#include "gmock\gmock.h"

class SerializableMock : public srl::ISerializable
{
	public:
		MOCK_CONST_METHOD0(getAsString, std::string());
		MOCK_METHOD1(initFromString, bool(const std::string &));
};