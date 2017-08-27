#include "CyclicBuffer_fixture.h"

using namespace testing;
using namespace srl;

CyclicBuffer_fixture::CyclicBuffer_fixture()
{
}

CyclicBuffer_fixture::~CyclicBuffer_fixture()
{
}

void CyclicBuffer_fixture::SetUp()
{

}

void CyclicBuffer_fixture::TearDown()
{

}

TEST_F(CyclicBuffer_fixture, consturctor_set_bufferSize)
{
	auto SIZE = 22u;
	CyclicBuffer buffer(SIZE);

	EXPECT_EQ(SIZE, buffer.getBufferSize());
}