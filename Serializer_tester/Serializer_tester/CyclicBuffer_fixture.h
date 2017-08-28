#pragma once
#include <gmock\gmock.h>
#include "Constants.h"
#include "Utils.h"

#include "CyclicBufferTestable.h"

class CyclicBuffer_fixture : public testing::Test
{
public:
	CyclicBuffer_fixture();
	~CyclicBuffer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

