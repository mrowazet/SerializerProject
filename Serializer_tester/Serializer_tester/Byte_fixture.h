#pragma once
#include <gtest\gtest.h>
#include "TestConstants.h"
#include <Byte.h>

class Byte_fixture : public testing::Test
{
	public:
		Byte_fixture();
		virtual ~Byte_fixture();

		virtual void SetUp() override;
		virtual void TearDown() override;

	protected:

};

