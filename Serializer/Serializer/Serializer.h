#pragma once
#include "ISerializer.h"

namespace srl
{

class Serializer : public ISerializer
{
	friend class SimpleFileHandler;

	public:
		Serializer();
		Serializer(const ByteArray & byteArray);
		Serializer(Serializer && serializer);
		virtual ~Serializer();

		Serializer & operator=(Serializer && serializer);

		Serializer(const Serializer & serializer) = delete;
		Serializer & operator=(const Serializer & serializer) = delete;

		virtual void clear() override;
		virtual unsigned int size() const override;
		virtual bool isEmpty() const override;
	    virtual operator bool() override;

		virtual const ByteArray & getData() const override;

		virtual int getWriteIndex() const override;
		virtual int getReadIndex() const override;

		virtual bool setWriteIndex(const int & index) override;
		virtual bool setReadIndex(const int & index) const override;

		virtual Byte_8 & at(const unsigned int & index) override;
		virtual const Byte_8 & at(const unsigned int & index) const override;

		Serializer & operator<<(const ByteArray & byteArray);
		Serializer & operator<<(const char * c_str);

		Serializer & operator<<(const std::string & value);
		const Serializer & operator>>(std::string & value) const;

		virtual Serializer & operator<<(const ISerializable & serializable) override;
		virtual const Serializer & operator>>(ISerializable & serializable) const override;

		template <typename T>
		Serializer & operator<<(const T & value);

		template <typename T>
		const Serializer & operator>>(T & value) const;

	private:
		ByteArray m_data;

		unsigned int m_writeIndex = 0;
		mutable unsigned int m_readIndex = 0;

		unsigned int bufferSize() const;
		bool isReadIndexCorrect(const unsigned int & index) const;
		bool isWriteIndexCorrect(const unsigned int & index) const;
		void moveSerializerContent(Serializer && serializer);

		template<typename T>
		void storeDataInBuffer(const T & data);

		template<typename T>
		void getDataFromBuffer(T & data) const;
};

template <typename T>
Serializer & Serializer::operator<<(const T & value)
{
	storeDataInBuffer<T>(value);
	return *this;
}

template <typename T>
const Serializer & Serializer::operator>>(T & value) const
{
	getDataFromBuffer<T>(value);
	return *this;
}

template<typename T>
void Serializer::storeDataInBuffer(const T & data)
{
	const int RequiredExtraBytes = (m_writeIndex + sizeof(T)) - bufferSize();

	if (RequiredExtraBytes > 0)
		m_data.resize(m_data.size() + RequiredExtraBytes);

	const char * singleByte = reinterpret_cast<const char*>(&data);

	for (int i = 0; i < sizeof(T); i++)
		m_data[m_writeIndex + i] = *(singleByte + i);

	m_writeIndex += sizeof(T);
}

template<typename T>
void Serializer::getDataFromBuffer(T & data) const
{
	if (bufferSize() == 0)
	{
		std::cout << "Empty! Not possible to read! \n";
		return;
	}

	if (m_readIndex + sizeof(T) > bufferSize())
	{
		std::cout << "Stored data too short! Not possible to read! \n";
		return;
	}

	const T* typedValue = reinterpret_cast<const T*>(&m_data[m_readIndex]);
	data = *typedValue;

	m_readIndex += sizeof(T);
}

} //end of namespace