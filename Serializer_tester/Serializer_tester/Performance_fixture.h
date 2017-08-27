#pragma once
#include <gtest\gtest.h>
#include "Constants.h"
#include "Stopwatch.h"
#include <array>
#include <vector>

class Performance_fixture : public testing::Test
{
	public:
		Performance_fixture();
		~Performance_fixture();

		virtual void SetUp() override;
		virtual void TearDown() override;

	protected:
		Stopwatch m_stopwatch;
};

