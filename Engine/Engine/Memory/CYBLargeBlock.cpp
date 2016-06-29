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

CYB::Engine::Memory::LargeBlock::LargeBlock(const unsigned long long ASpaceAvailable, Block* const ALeftBlock) noexcept :
	Block(sizeof(Block), ALeftBlock == nullptr ? *this : *ALeftBlock, true),
#ifdef DEBUG
	FMagicHeader(MAGIC_HEADER),
#endif
	FRemainingSize(ASpaceAvailable)
#ifdef DEBUG
	,FMagicFooter(MAGIC_FOOTER)
#endif
{}

CYB::Engine::Memory::Block& CYB::Engine::Memory::LargeBlock::AllocateBlock(LargeBlock*& ALargeBlock, const unsigned int ANewBlockSize) noexcept {
	const auto SizeWithBlock(ANewBlockSize + sizeof(Block));
	const auto OriginalSize(ALargeBlock->FRemainingSize);
	API::Assert::LessThan<unsigned long long>(SizeWithBlock, OriginalSize);
	Block& NewBlock(*API::Interop::Allocator::InPlaceAllocation<Block>(ALargeBlock, static_cast<unsigned int>(SizeWithBlock), ALargeBlock->LeftBlock() == nullptr ? *ALargeBlock : *ALargeBlock->LeftBlock(), false));
	ALargeBlock = API::Interop::Allocator::InPlaceAllocation<LargeBlock>(reinterpret_cast<byte*>(&NewBlock) + SizeWithBlock, OriginalSize - SizeWithBlock, &NewBlock);
	return NewBlock;
}

void CYB::Engine::Memory::LargeBlock::Validate(void) const {
	API::Assert::Equal<unsigned long long>(FMagicHeader, MAGIC_HEADER);
	API::Assert::Equal<unsigned long long>(FMagicFooter, MAGIC_FOOTER);
	Block::Validate();
}

CYB::Engine::Memory::LargeBlock& CYB::Engine::Memory::LargeBlock::EatLeftBlock(void) noexcept {
	API::Assert::NotEqual<Block*>(LeftBlock(), nullptr);
	return *API::Interop::Allocator::InPlaceAllocation<LargeBlock>(LeftBlock(), FRemainingSize + LeftBlock()->Size() + sizeof(Block), LeftBlock()->LeftBlock());	//Even though we are eating a LargeBlock header, we are are also creating a LargeBlock Header - A block header
}

unsigned long long CYB::Engine::Memory::LargeBlock::Size(void) const noexcept {
	return FRemainingSize;
}

void CYB::Engine::Memory::LargeBlock::SetSize(const unsigned long long ANewSize) noexcept {
	FRemainingSize = ANewSize;
}