#pragma once
#include "ActiveSerializer.h"

namespace srl
{

class ActiveSerializerTestable : public ActiveSerializer
{
public:
	ActiveSerializerTestable();
	~ActiveSerializerTestable();

	using ActiveSerializer::ASSERT_FILE_OPENED;

	ActiveSerializerTestable(ActiveSerializerTestable && serializer) = default;
	ActiveSerializerTestable & operator=(ActiveSerializerTestable && serializer) = default;

	virtual bool openFile(const Path & dir, const IOMode & mode = IOMode::Truncate) override;
	virtual void closeFile() override;

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

	virtual ActiveSerializerTestable & operator<<(const ByteArray & byteArray) override;
	virtual ActiveSerializerTestable & operator<<(const char * c_str) override;

	virtual ActiveSerializerTestable & operator<<(const std::string & value) override;
	virtual const ActiveSerializerTestable & operator>>(std::string & value) const override;

	virtual ActiveSerializerTestable & operator<<(const ISerializable & serializable) override;
	virtual const ActiveSerializerTestable & operator>>(ISerializable & serializable) const override;
};

} //end of namespace
