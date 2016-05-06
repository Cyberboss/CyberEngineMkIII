//! @file CYBLargeBlock.cpp Implements functions from CYB::Engine::LargeBlock
#include "CYB.hpp"
#ifdef TARGET_OS_WINDOWS
#pragma warning(disable : 4355) //this used in initalizer list
#endif

CYB::Engine::LargeBlock::LargeBlock(const unsigned long long ASpaceAvailable) :
	Block(0, *this, true),
	FMagicHeader(MAGIC_HEADER),
	FLargeBlockToTheRight(nullptr),
	FRemainingSize(ASpaceAvailable),
	FMagicFooter(MAGIC_FOOTER)
{}