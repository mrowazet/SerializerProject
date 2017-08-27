#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"

#include "CyclicBuffer.h"

class CyclicBuffer_fixture : public testing::Test
{
public:
	CyclicBuffer_fixture();
	~CyclicBuffer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

