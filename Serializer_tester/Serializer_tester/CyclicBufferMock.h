#pragma once
#include "gmock\gmock.h"
#include "ICyclicBuffer.h"

namespace srl 
{
	class CyclicBufferMock : public ICyclicBuffer
	{
	public:
		MOCK_CONST_METHOD0(size, unsigned int());
		MOCK_CONST_METHOD0(isEmpty, bool());
		MOCK_METHOD0(clear, void());
		MOCK_CONST_METHOD0(data, srl::Byte_8&());

		virtual operator bool() const override
		{
			return isEmpty();
		}
	};

} //end of namespace