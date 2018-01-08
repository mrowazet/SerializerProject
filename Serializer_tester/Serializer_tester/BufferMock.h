#pragma once
#include "gmock\gmock.h"
#include "IBuffer.h"

namespace srl 
{
	class BufferMock : public IBuffer
	{
	public:
		MOCK_CONST_METHOD0(size, unsigned int());
		MOCK_CONST_METHOD0(isEmpty, bool());
		MOCK_METHOD0(clear, void());
		MOCK_METHOD0(clearIndexes, void());
		MOCK_CONST_METHOD0(data, const srl::ByteArray&());

		MOCK_METHOD1(write, void(const unsigned int&));
		MOCK_METHOD1(write, void(const Byte_8&));
		MOCK_METHOD1(write, void(const ByteArray&));

		MOCK_CONST_METHOD0(read, Byte_8());
		MOCK_CONST_METHOD1(read, ByteArray(const int&));

		MOCK_CONST_METHOD0(getWriteIndex, int());
		MOCK_CONST_METHOD0(getReadIndex, int());

		MOCK_METHOD1(setWriteIndex, void(const int&));
		MOCK_CONST_METHOD1(setReadIndex, void(const int&));

		MOCK_METHOD0(begin, Iter());
		MOCK_CONST_METHOD0(begin, CIter());
		MOCK_CONST_METHOD0(cbegin, CIter());

		MOCK_METHOD0(end, Iter());
		MOCK_CONST_METHOD0(end, CIter());
		MOCK_CONST_METHOD0(cend, CIter());

		virtual operator bool() const override
		{
			return isEmpty();
		}
	};

} //end of namespace