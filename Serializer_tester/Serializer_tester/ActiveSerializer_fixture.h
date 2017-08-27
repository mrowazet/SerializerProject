#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"

#include "ActiveSerializerTestable.h"

class ActiveSerializer_fixture : public testing::Test
{
public:
	ActiveSerializer_fixture();
	~ActiveSerializer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

