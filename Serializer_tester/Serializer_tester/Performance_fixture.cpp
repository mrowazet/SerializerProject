#include "Performance_fixture.h"

#include <experimental\filesystem>
#include "Utils.h"
#include "ByteArray.h"
#include "Serializer.h"
#include "SimpleIO.h"
#include "BufferTestable.h"

using namespace Utils;

namespace
{
	const bool ON = true;
	const bool OFF = false;

	bool ALL_TC_ENABLED = OFF;

	//TESTS 
	bool ByteArray_resize_zero_to_value = OFF;
	bool Add_new_double_to_serializer_three_bytes_more_needed = OFF;
	bool Add_ByteArray_to_serializer = OFF;
	bool Add_string_to_serializer = OFF;
	bool Get_string_from_serializer = OFF;
	bool ByteArray_getAsString = OFF;
	bool Append_ByteArray_to_ByteArray = OFF;
	bool Add_c_str_to_serializer = OFF;
	bool Load_data_from_file_using_SimpleIO = OFF;
	bool Clear_data_in_buffer = OFF;
}

using namespace ::srl;

Performance_fixture::Performance_fixture()
{

}

Performance_fixture::~Performance_fixture()
{

}

void Performance_fixture::SetUp()
{
	if (ALL_TC_ENABLED)
		enableAllTCs();
}

void Performance_fixture::TearDown()
{
	Tools::cleanUpTestFilesAndDirectories();
}

void Performance_fixture::enableAllTCs()
{
	ByteArray_resize_zero_to_value = ON;
	Add_new_double_to_serializer_three_bytes_more_needed = ON;
	Add_ByteArray_to_serializer = ON;
	Add_string_to_serializer = ON;
	Get_string_from_serializer = ON;
	ByteArray_getAsString = ON;
	Append_ByteArray_to_ByteArray = ON;
	Add_c_str_to_serializer = ON;
	Load_data_from_file_using_SimpleIO = ON;
	Clear_data_in_buffer = ON;
}

TEST_F(Performance_fixture, ByteArray_resize_zero_to_value)
{
	if (!ByteArray_resize_zero_to_value)
		return;

	const unsigned int Loops = 60000;	
	std::array<int, 4> destSizes{ 5, 10, 20, 40 };

	for (auto & size : destSizes)
	{
		std::cout << "\nLoops: " << Loops << " Start test with size: " << size << "\n";

		m_stopwatch.start();

		for (int i = 0; i < Loops; ++i)
		{
			ByteArray byteArray;
			byteArray.resize(size);				
		}

		m_stopwatch.stop();

		auto time = m_stopwatch.getLastElapsedTime();
		double elapsed = (double)time / Loops;
		std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";	
	}
}

TEST_F(Performance_fixture, Add_new_double_to_serializer_three_bytes_more_needed)
{
	if (!Add_new_double_to_serializer_three_bytes_more_needed)
		return;

	const unsigned int Loops = 60000;
	const int WriteIndexOffset = 3;
	const double TestValue = 424294.8;
	
	ByteArray byteArray(FOUR_BYTES);
	Byte_8 byte("11001100");

	byteArray << byte << byte << byte << byte;

	const int WriteIndex = byteArray.size() - WriteIndexOffset;

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		Serializer serializer(byteArray);

		serializer.setWriteIndex(WriteIndex);
		serializer << TestValue;
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Add_ByteArray_to_serializer)
{
	if (!Add_ByteArray_to_serializer)
		return;

	const unsigned int Loops = 10000;
	const unsigned int BytesInArray = 70;

	const Byte_8 Byte("10101010");
	ByteArray byteArray;

	for (unsigned int i = 0; i < BytesInArray; i++)
		byteArray << Byte;

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		Serializer serializer;
		serializer << byteArray;
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Bytes in ByteArray: " << BytesInArray << "\n";
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Add_string_to_serializer)
{
	if (!Add_string_to_serializer)
		return;

	const unsigned int Loops = 10000;
	const std::string TestString = "This is string to calculate performance of Serializer::operator<<------------------";

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		Serializer serializer;
		serializer << TestString;
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Size of string used to test: " << TestString.size() << "\n";
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Get_string_from_serializer)
{
	if (!Get_string_from_serializer)
		return;

	const unsigned int Loops = 100;
	const unsigned int StringVectorSize = 200;
	const std::string TestString =  "This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"
									"This is string to calculate performance of Serializer::operator>>(const std::string&)"; //1020 characters

	Serializer serializer;
	serializer << TestString;

	std::vector<std::string> strings;
	strings.resize(StringVectorSize);

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		for (auto & s : strings)
		{
			serializer >> s;
			serializer.setReadIndex(BYTE_0);
		}
			
		for (auto & s : strings)
			s.clear();
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / (Loops * StringVectorSize);
	std::cout << "Size of string used to test: " << TestString.size() << "\n";
	std::cout << "Size of string vector: " << StringVectorSize << "\n";
	std::cout << "Test done. Loops: " << Loops * StringVectorSize << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, ByteArray_getAsString)
{
	if (!ByteArray_getAsString)
		return;

	const unsigned int Loops = 2000;
	const unsigned int Bytes = 500;

	ByteArray byteArray(Bytes);

	const int ExtraSize = 50;
	std::string output;
	output.resize(Bytes + ExtraSize);

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		output = byteArray.getAsString();
		output.clear();
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Size of byteArray used to test: " << byteArray.size() << "\n";
	std::cout << "Test done. Loops: " << Loops  << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Append_ByteArray_to_ByteArray)
{
	if (!Append_ByteArray_to_ByteArray)
		return;

	const unsigned int Loops = 10000;
	const unsigned int Bytes = 600;

	const ByteArray byteArrayToAdd(Bytes);

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		ByteArray byteArray(Bytes);
		byteArray += byteArrayToAdd;
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Size of byteArray used to test: " << byteArrayToAdd.size() << "\n";
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Add_c_str_to_serializer)
{
	if (!Add_c_str_to_serializer)
		return;

	const unsigned int Loops = 10000;
	const std::string TestString = "This is string to calculate performance of Serializer::operator<<------------------";
	auto c_str = TestString.c_str();

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		Serializer serializer;
		serializer << c_str;
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Size of string used to test: " << TestString.size() << "\n";
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Load_data_from_file_using_SimpleIO)
{
	if (!Load_data_from_file_using_SimpleIO)
		return;

	const unsigned int Loops = 10000;
	std::string TestString = "This is string to calculate performance of SimpleIO::loadFromBinFile function";
	TestString += "LINE1: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	TestString += "LINE2: bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
	TestString += "LINE3: ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

	Serializer serializer;
	serializer << TestString;

	SimpleIO::saveToBinFile(serializer);
	serializer.clear();
	SimpleIO::loadFromBinFile(serializer);

	std::string loadedString;
	serializer >> loadedString;
	ASSERT_EQ(TestString, loadedString);

	serializer.clear();

	//Test
	m_stopwatch.start();

	for (int i = 0; i < Loops; ++i)
	{
		SimpleIO::loadFromBinFile(serializer);
		serializer.clear();
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Size of string used to test: " << TestString.size() << "\n";
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}

TEST_F(Performance_fixture, Clear_data_in_buffer)
{
	if (!Clear_data_in_buffer)
		return;

	const unsigned int Loops = 30000;

	//Test
	m_stopwatch.start();
	BufferTestable buffer(SERIALIZER_BUFFER_MAX);

	for (int i = 0; i < Loops; ++i)
	{
		buffer.clear();
	}

	m_stopwatch.stop();

	auto time = m_stopwatch.getLastElapsedTime();
	double elapsed = (double)time / Loops;
	std::cout << "Test done. Loops: " << Loops << " Average time per loop: " << elapsed << "ms \n\n";
}


