#pragma once
#include "ISerializer.h"

namespace srl
{

class FlySerializer : public ISerializer
{
public:
	FlySerializer();
	virtual ~FlySerializer();

	FlySerializer(FlySerializer && serializer);
	FlySerializer & operator=(FlySerializer && serializer);

	FlySerializer(const FlySerializer & serializer) = delete;
	FlySerializer & operator=(const FlySerializer & serializer) = delete;

	virtual void clear() override;
	virtual unsigned int size() const override;
	virtual bool isEmpty() const override;
	virtual operator bool() const override;

	virtual const ByteArray & getData() const override;

	virtual int getWriteIndex() const override;
	virtual int getReadIndex() const override;

	virtual bool setWriteIndex(const int & index) override;
	virtual bool setReadIndex(const int & index) const override;

	virtual Byte_8 & at(const unsigned int & index) override;
	virtual const Byte_8 & at(const unsigned int & index) const override;

	virtual FlySerializer & operator<<(const ByteArray & byteArray) override;
	virtual FlySerializer & operator<<(const char * c_str) override;

	virtual FlySerializer & operator<<(const std::string & value) override;
	virtual const FlySerializer & operator>>(std::string & value) const override;

	virtual FlySerializer & operator<<(const ISerializable & serializable) override;
	virtual const FlySerializer & operator>>(ISerializable & serializable) const override;

private:


};

} //end of namespace