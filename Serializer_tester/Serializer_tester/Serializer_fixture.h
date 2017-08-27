#pragma once
#include <gtest\gtest.h>
#include "Serializer.h"
#include "TestConstants.h"
#include "Stopwatch.h"
#include "SerializableMock.h"
#include "SerializableTestClass.h"

class Serializer_fixture : public testing::Test
{
	public:
		Serializer_fixture();
		virtual ~Serializer_fixture();

		virtual void SetUp() override;
		virtual void TearDown() override;
};

