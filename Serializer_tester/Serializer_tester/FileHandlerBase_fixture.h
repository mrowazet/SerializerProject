#pragma once
#include <gtest\gtest.h>
#include "Utils.h"
#include "FileHandlerBase.h"

class FileHandlerBase_fixture : public testing::Test
{
public:
	FileHandlerBase_fixture();
	~FileHandlerBase_fixture();

	virtual void SetUp() override;
	virtual void TearDown() override;

protected:
	std::unique_ptr<std::ifstream> m_inFile;
	std::unique_ptr<std::ofstream> m_outFile;
	std::unique_ptr<std::fstream> m_inOutFile;
};

