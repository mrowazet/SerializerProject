#include "BufferedDataInfo.h"

namespace srl
{

BufferedDataInfo::BufferedDataInfo()
{
}

BufferedDataInfo::~BufferedDataInfo()
{
}

BufferedDataInfo::IndexPosition BufferedDataInfo::getLastCorrectWriteIndex() const
{
	return m_lastCorrectWriteIndex;
}

BufferedDataInfo::IndexPosition BufferedDataInfo::getLastCorrectReadIndex() const
{
	return m_lastCorrectReadIndex;
}

BufferedDataInfo::IndexPosition BufferedDataInfo::getBeginIndexRelativelyToFile() const
{
	return m_beginIndexRelativelyToFile;
}

void BufferedDataInfo::setBeginIndex(const IndexPosition & index) const
{
	m_beginIndexRelativelyToFile = index;
}

void BufferedDataInfo::updateAccessIndexesByAddedDataSize(const unsigned int & addedDataSize) const
{
	if (areAccessIndexesCleared())
	{
		m_lastCorrectWriteIndex = addedDataSize;
		m_lastCorrectReadIndex = addedDataSize - 1;
	}
	else
	{
		m_lastCorrectWriteIndex += addedDataSize;
		m_lastCorrectReadIndex += addedDataSize;
	}
}

void BufferedDataInfo::clearAccessIndexes() const
{
	m_lastCorrectReadIndex = 0;
	m_lastCorrectWriteIndex = 0;
}

bool BufferedDataInfo::areAccessIndexesCleared() const
{
	if (m_lastCorrectReadIndex == 0 && m_lastCorrectWriteIndex == 0)
		return true;
	else
		return false;
}

void BufferedDataInfo::clear() const
{
	clearAccessIndexes();
	clearBeginIndex();
}

bool BufferedDataInfo::isCleared() const
{
	if (areAccessIndexesCleared() && isBeginIndexCleared())
		return true;
	else
		return false;
}

void BufferedDataInfo::clearBeginIndex() const
{
	m_beginIndexRelativelyToFile = 0;
}

bool BufferedDataInfo::isBeginIndexCleared() const
{
	if (m_beginIndexRelativelyToFile == 0)
		return true;
	else
		return false;
}

} //end of namespace
