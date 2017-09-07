#pragma once
#include <gmock\gmock.h>
#include "Constants.h"
#include "Utils.h"

#include "BufferTestable.h"

class Buffer_fixture : public testing::Test
{
public:
	Buffer_fixture();
	~Buffer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;
};

