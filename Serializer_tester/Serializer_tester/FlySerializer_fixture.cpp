#include "FlySerializer_fixture.h"

using namespace ::testing;
using namespace ::srl;

FlySerializer_fixture::FlySerializer_fixture()
{
}

FlySerializer_fixture::~FlySerializer_fixture()
{
}

void FlySerializer_fixture::SetUp()
{

}

void FlySerializer_fixture::TearDown()
{
	ut::cleanUpTestFilesAndDirectories();
}
