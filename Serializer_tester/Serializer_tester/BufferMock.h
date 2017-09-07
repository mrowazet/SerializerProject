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
		MOCK_CONST_METHOD0(data, srl::Byte_8&());

		MOCK_METHOD1(write, void(const Byte_8&));
		MOCK_METHOD1(write, void(const ByteArray&));

		MOCK_CONST_METHOD0(read, Byte_8());
		MOCK_CONST_METHOD1(read, ByteArray(const int&));

		virtual operator bool() const override
		{
			return isEmpty();
		}
	};

} //end of namespace