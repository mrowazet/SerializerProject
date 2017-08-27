#pragma once
#include <gtest\gtest.h>
#include "Serializer.h"
#include "SimpleIO.h"
#include "Utils.h"
#include <vector>

class SimpleIO_fixture : public testing::Test
{
	using Path = std::experimental::filesystem::path;

public:
	SimpleIO_fixture();
	~SimpleIO_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;

protected:

	const struct TestData
	{
		std::vector<int> intValues;
		std::vector<char> charValues;
		std::vector<double> doubleValues;
		std::vector<float> floatValues;

		std::string c_strValue;
		std::string stringValue;

		TestData();
	} testData;

	srl::ByteArray makeByteArrayForTest();

	const srl::Serializer SERIALIZER;
};

