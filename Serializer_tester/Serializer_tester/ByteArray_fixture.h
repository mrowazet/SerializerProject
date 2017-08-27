#pragma once
#include <gtest\gtest.h>
#include <ByteArray.h>
#include "TestConstants.h"

class ByteArray_fixture : public testing::Test
{
	public:
		ByteArray_fixture();
		virtual ~ByteArray_fixture();

		virtual void SetUp() override;
		virtual void TearDown() override;

	protected:

};

