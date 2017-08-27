#include "Utilities_fixture.h"


Utilities_fixture::Utilities_fixture()
{
}

Utilities_fixture::~Utilities_fixture()
{
}

void Utilities_fixture::SetUp()
{
}

void Utilities_fixture::TearDown()
{
}

TEST_F(Utilities_fixture, should_return_true_when_default_dir_is_used)
{
	EXPECT_TRUE(srl::Utilities::isDefaultDirectory(srl::DEFAULT_DIRECTORY));
}

TEST_F(Utilities_fixture, should_return_false_when_not_default_dir_is_used)
{
	srl::Path notDefaultDir = srl::Path("anotherDirectory\\file.bin");
	EXPECT_FALSE(srl::Utilities::isDefaultDirectory(notDefaultDir));
}
