#pragma once
#include "Constants.h"
#include "Byte.h"
#include "ByteArray.h"

namespace srl
{

class ICyclicBuffer
{
public:
	ICyclicBuffer() = default;
	virtual ~ICyclicBuffer() = default;

	//TODO
	// rest of special operators

	virtual unsigned int size() const = 0;
	virtual bool isEmpty() const = 0;
	virtual operator bool() const = 0;
	virtual void clear() = 0;

	virtual const srl::Byte_8 & data() const = 0;
};

} //end of namespace