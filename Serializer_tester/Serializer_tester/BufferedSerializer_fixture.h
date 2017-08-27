#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"
#include "BufferedSerializer.h"

class BufferedSerializer_fixture : public testing::Test
{
public:
	BufferedSerializer_fixture();
	~BufferedSerializer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

