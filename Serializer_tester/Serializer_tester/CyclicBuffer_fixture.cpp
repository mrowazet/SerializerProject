#include "CyclicBuffer_fixture.h"

using namespace testing;
using namespace srl;

class CyclicBufferTestable : public srl::CyclicBuffer
{
public:
	CyclicBufferTestable(const unsigned int & bufferSize = SERIALIZER_BUFFER_MIN)
		:CyclicBuffer(bufferSize),
		 internalData(m_data)
	{

	}

	ByteArray& internalData;
};

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

	EXPECT_EQ(SIZE, buffer.size());
}

TEST_F(CyclicBuffer_fixture, default_constructor_set_buffer_size_for_min)
{
	CyclicBuffer buffer;

	EXPECT_EQ(SERIALIZER_BUFFER_MIN, buffer.size());
}

TEST_F(CyclicBuffer_fixture, getData_returns_correct_address)
{
	CyclicBufferTestable buffer(SERIALIZER_BUFFER_MIN);

	auto data = buffer.data();
	EXPECT_EQ(buffer.internalData[0], data);
}

TEST_F(CyclicBuffer_fixture, isEmpty_returns_false_if_any_bit_is_1)
{
	const Byte_8 BYTE("00000001");
	CyclicBufferTestable buffer;
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	EXPECT_FALSE(buffer.isEmpty());
}

TEST_F(CyclicBuffer_fixture, isEmpty_returns_true_if_all_bits_are_0)
{
	CyclicBuffer buffer;

	EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(CyclicBuffer_fixture, boolOperator_returns_same_value_as_isEmpty)
{
	CyclicBufferTestable buffer;

	EXPECT_TRUE(buffer);

	const Byte_8 BYTE("00000001");
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	EXPECT_FALSE(buffer);
}

TEST_F(CyclicBuffer_fixture, clear_erase_all_data_in_internal_buffer)
{
	const Byte_8 BYTE("01010101");
	CyclicBufferTestable buffer;
	buffer.internalData[SERIALIZER_BUFFER_MIN - 1] = BYTE;

	ASSERT_FALSE(buffer.isEmpty());

	buffer.clear();
	EXPECT_TRUE(buffer.isEmpty());
}
