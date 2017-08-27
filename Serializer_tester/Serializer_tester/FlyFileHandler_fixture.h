#pragma once
#include <gtest\gtest.h>
#include "TestConstants.h"
#include <Byte.h>

class FlyFileHandler_fixture : public testing::Test
{
public:
	FlyFileHandler_fixture();
	~FlyFileHandler_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

