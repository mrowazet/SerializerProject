#pragma once

namespace srl
{

class BufferedDataInfo
{
public:
	using IndexPosition = unsigned int;

	BufferedDataInfo();
	~BufferedDataInfo();

	IndexPosition getLastCorrectWriteIndex() const;
	IndexPosition getLastCorrectReadIndex() const;
	IndexPosition getBeginIndexRelativelyToFile() const;

	void setBeginIndex(const IndexPosition& index) const;
	void updateAccessIndexesByAddedDataSize(const unsigned int& addedDataSize) const;

	void clearAccessIndexes() const;
	bool areAccessIndexesCleared() const;

	void clear() const;
	bool isCleared() const;

private:
	mutable IndexPosition m_lastCorrectWriteIndex = 0;
	mutable IndexPosition m_lastCorrectReadIndex = 0;
	mutable IndexPosition m_beginIndexRelativelyToFile = 0;

	void clearBeginIndex() const;
	bool isBeginIndexCleared() const;
};

} //end of namespace

