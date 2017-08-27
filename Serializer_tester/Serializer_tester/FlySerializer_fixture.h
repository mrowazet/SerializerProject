#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Utils.h"
#include "FlySerializer.h"

class FlySerializer_fixture : public testing::Test
{
public:
	FlySerializer_fixture();
	virtual ~FlySerializer_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;

protected:
};

