#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"
#include "BufferedDataInfo.h"

class BufferedDataInfo_fixture : public testing::Test
{
public:
	BufferedDataInfo_fixture();
	~BufferedDataInfo_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

