//! @file CYBBlock.cpp Implements functions from CYB::Engine::Block
#include "CYB.hpp"

CYB::Engine::Block::Block(const int ASpaceAvailable, const Block& ALeftBlock, const bool AFree) :
	FMagicHeader(MAGIC_HEADER),
	FSizeAndFreeBit(InitializeData(ASpaceAvailable, AFree)),
	FOffsetToPreviousBlock(CalculateOffset(ALeftBlock)),
	FMagicFooter(MAGIC_FOOTER)
{}

unsigned int CYB::Engine::Block::InitializeData(const int ASize, const bool AFree) {
	API::Assert(ASize >= 0);
	if(AFree)
		return ASize | 1U << 31;
	else
		return ASize & ~(1U << 31);
}

unsigned int CYB::Engine::Block::CalculateOffset(const Block& ABlock) {
	const auto Result(static_cast<unsigned long long>(&ABlock > this ? &ABlock - this : this - &ABlock));
	//! @todo This could cause trouble
	API::Assert(Result <= std::numeric_limits<unsigned int>().max());
	return static_cast<unsigned int>(Result);
}