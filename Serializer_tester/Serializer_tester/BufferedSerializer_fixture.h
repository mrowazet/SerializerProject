#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"
#include "BufferedSerializerTestable.h"

class BufferedSerializer_fixture : public ::testing::Test
{
public:
	BufferedSerializer_fixture();
	~BufferedSerializer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;

	srl::BufferedSerializerTestable makeSerializerWithDefaultDirOpened(const srl::IOMode openMode = srl::IOMode::Truncate);
	srl::ByteArray makeByteArray(const unsigned int& size, srl::Byte_8 byte = srl::Byte_8("10101010"));
};

