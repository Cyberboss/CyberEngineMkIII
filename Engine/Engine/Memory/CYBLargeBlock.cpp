//! @file CYBLargeBlock.cpp Implements functions from CYB::Engine::LargeBlock
#include "CYB.hpp"
#ifdef TARGET_OS_WINDOWS
#pragma warning(disable : 4355) //this used in initalizer list
#endif

static_assert(sizeof(CYB::Engine::Memory::LargeBlock) - sizeof(CYB::Engine::Memory::Block) ==
#ifdef DEBUG
	24
#else
	8
#endif
	, "LargeBlock size has changed, check algorithms");

CYB::Engine::Memory::LargeBlock::LargeBlock(const unsigned long long ASpaceAvailable, Block* const ALeftBlock) :
	Block(sizeof(Block), ALeftBlock == nullptr ? *this : *ALeftBlock, true),
	FMagicHeader(MAGIC_HEADER),
	FRemainingSize(ASpaceAvailable),
	FMagicFooter(MAGIC_FOOTER)
{}

CYB::Engine::Memory::Block& CYB::Engine::Memory::LargeBlock::AllocateBlock(LargeBlock*& ALargeBlock, const unsigned int ANewBlockSize) {
	const auto SizeWithBlock(ANewBlockSize + sizeof(Block));
	const auto OriginalSize(ALargeBlock->FRemainingSize);
	API::Assert(OriginalSize > SizeWithBlock);
	Block& NewBlock(*new (ALargeBlock) Block(static_cast<unsigned int>(SizeWithBlock), ALargeBlock->LeftBlock() == nullptr ? *ALargeBlock : *ALargeBlock->LeftBlock(), false));
	ALargeBlock = new (reinterpret_cast<byte*>(&NewBlock) + SizeWithBlock) LargeBlock(OriginalSize - SizeWithBlock, &NewBlock);
	return NewBlock;
}

void CYB::Engine::Memory::LargeBlock::Validate(void) const {
#ifdef DEBUG
	if (FMagicHeader != MAGIC_HEADER || FMagicFooter != MAGIC_FOOTER)
		throw CYB::Exception::Violation(CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK);
#endif
	Block::Validate();
}

CYB::Engine::Memory::LargeBlock& CYB::Engine::Memory::LargeBlock::EatLeftBlock(void) {
	API::Assert(LeftBlock() != nullptr);
	return *new (LeftBlock()) LargeBlock(FRemainingSize + LeftBlock()->Size() + sizeof(Block), LeftBlock()->LeftBlock());	//Even though we are eating a LargeBlock header, we are are also creating a LargeBlock Header - A block header
}

unsigned long long CYB::Engine::Memory::LargeBlock::Size(void) const {
	return FRemainingSize;
}

void CYB::Engine::Memory::LargeBlock::SetSize(const unsigned long long ANewSize) {
	FRemainingSize = ANewSize;
}