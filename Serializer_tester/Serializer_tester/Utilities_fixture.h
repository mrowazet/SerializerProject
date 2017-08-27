#pragma once
#include <gtest\gtest.h>
#include "TestConstants.h"
#include "Utilities.h"
#include <Byte.h>

class Utilities_fixture : public testing::Test
{
public:
	Utilities_fixture();
	~Utilities_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

