//! @file CYBLargeBlock.cpp Implements functions from CYB::Engine::LargeBlock
#include "CYB.hpp"
#ifdef TARGET_OS_WINDOWS
#pragma warning(disable : 4355) //this used in initalizer list
#endif

static_assert(sizeof(CYB::Engine::LargeBlock) - sizeof(CYB::Engine::Block) ==
#ifdef DEBUG
	32
#else
	16
#endif
	, "LargeBlock size has changed, check algorithms");

CYB::Engine::LargeBlock::LargeBlock(const unsigned long long ASpaceAvailable) :
	Block(0, *this, true),
	FMagicHeader(MAGIC_HEADER),
	FLargeBlockToTheRight(nullptr),
	FRemainingSize(ASpaceAvailable),
	FMagicFooter(MAGIC_FOOTER)
{}
CYB::Engine::LargeBlock::LargeBlock(const unsigned long long ASpaceAvailable, LargeBlock* ALargeBlockToTheRight) :
	Block(0, *this, true),
	FMagicHeader(MAGIC_HEADER),
	FLargeBlockToTheRight(ALargeBlockToTheRight),
	FRemainingSize(ASpaceAvailable),
	FMagicFooter(MAGIC_FOOTER)
{}

CYB::Engine::Block& CYB::Engine::LargeBlock::AllocateBlock(LargeBlock*& ALargeBlock, const int ANewBlockSize) {
	const auto SizeWithBlock(ANewBlockSize + sizeof(Block));
	const auto OriginalSize(ALargeBlock->FRemainingSize);
	API::Assert(OriginalSize > SizeWithBlock);
	ALargeBlock->SetSize(ANewBlockSize);
	ALargeBlock->SetFree(false);
	Block& NewBlock(*ALargeBlock);
	ALargeBlock = new (ALargeBlock->GetData()) LargeBlock(OriginalSize - SizeWithBlock, ALargeBlock->FLargeBlockToTheRight);
	return NewBlock;
}

void CYB::Engine::LargeBlock::Validate(void) const {
#ifdef DEBUG
	if (FMagicHeader != MAGIC_HEADER || FMagicFooter != MAGIC_FOOTER)
		throw CYB::Exception::Violation(CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK);
#endif
	Block::Validate();
}

CYB::Engine::LargeBlock& CYB::Engine::LargeBlock::EatLeftBlock(void) {
	API::Assert(LeftBlock() != nullptr);
	return *new (LeftBlock()) LargeBlock(FRemainingSize + LeftBlock()->Size() + sizeof(Block));	//Even though we are eating a LargeBlock header, we are are also creating a LargeBlock Header - A block header
}