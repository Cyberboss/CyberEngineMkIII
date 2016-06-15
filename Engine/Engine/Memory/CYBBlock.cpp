//! @file CYBBlock.cpp Implements functions from CYB::Engine::Block
#include "CYB.hpp"

static_assert(sizeof(CYB::Engine::Memory::Block) ==
#ifdef DEBUG
	32
#else
	16
#endif
	, "Block size has changed, check algorithms");

CYB::Engine::Memory::Block::Block(const unsigned int ASpaceAvailable, const Block& ALeftBlock, const bool AFree) noexcept :
#ifdef DEBUG
	FMagicHeader(MAGIC_HEADER),
#endif
	FSizeAndFreeBit(InitializeData(ASpaceAvailable, AFree)),
	FOffsetToPreviousBlock(CalculateOffset(ALeftBlock))
#ifdef DEBUG
	,FMagicFooter(MAGIC_FOOTER)
#endif
{
	API::Assert::LessThanOrEqual<unsigned long long>(sizeof(Block), ASpaceAvailable);
}

unsigned int CYB::Engine::Memory::Block::InitializeData(const unsigned int ASize, const bool AFree) noexcept {
	const auto DataSize(static_cast<long long>(ASize - sizeof(Block)));
	API::Assert::LessThanOrEqual<long long>(0, DataSize);
	API::Assert::LessThanOrEqual<long long>(DataSize, std::numeric_limits<int>().max());
	if(AFree)
		return static_cast<unsigned int>(DataSize) | 1U << 31;
	return static_cast<unsigned int>(DataSize) & ~(1U << 31);
}

unsigned int CYB::Engine::Memory::Block::CalculateOffset(const Block& ABlock) noexcept {
	API::Assert::LessThanOrEqual<const Block*>(&ABlock, this);
	const auto Result(reinterpret_cast<byte*>(this) - reinterpret_cast<const byte*>(&ABlock));
	API::Assert::LessThanOrEqual<long long>(Result, std::numeric_limits<unsigned int>().max());
	return static_cast<unsigned int>(Result);
}

unsigned int CYB::Engine::Memory::Block::Size(void) const noexcept {
	const auto Result(FSizeAndFreeBit & ~(1U << 31));
	API::Assert::LessThan(Result, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	return Result;
}

bool CYB::Engine::Memory::Block::IsFree(void) const noexcept {
	return (FSizeAndFreeBit >> 31) & 1;
}

bool CYB::Engine::Memory::Block::IsLargeBlock(void) const noexcept {
	return IsFree() && Size() == 0;
}

void* CYB::Engine::Memory::Block::GetData(void) noexcept {
	return this + 1;
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Block::FromData(void* const AData) {
	Block& TheBlock(*reinterpret_cast<Block*>(static_cast<byte*>(AData) - sizeof(Block)));
	TheBlock.Validate();
	return TheBlock;
}

CYB::Engine::Memory::Block* CYB::Engine::Memory::Block::LeftBlock(void) noexcept {
	if (FOffsetToPreviousBlock == 0)
		return nullptr;
	return reinterpret_cast<Block*>(reinterpret_cast<byte*>(this) - FOffsetToPreviousBlock);
}

CYB::Engine::Memory::Block* CYB::Engine::Memory::Block::RightBlock(void) noexcept {
	API::Assert::False(IsLargeBlock());
	return reinterpret_cast<Block*>(static_cast<byte*>(GetData()) + Size());
}

const CYB::Engine::Memory::Block* CYB::Engine::Memory::Block::RightBlock(void) const noexcept {
	API::Assert::False(IsLargeBlock());
	return reinterpret_cast<const Block*>(reinterpret_cast<const byte*>(this) + sizeof(Block) + Size());
}
void CYB::Engine::Memory::Block::SetSize(const unsigned int ANewSize) noexcept {
	API::Assert::LessThan(ANewSize, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	FSizeAndFreeBit = (ANewSize | 1U << 31);
}
void CYB::Engine::Memory::Block::SetFree(const bool ANewFree) noexcept {
	if(ANewFree)
		FSizeAndFreeBit |= 1U << 31;
	else
		FSizeAndFreeBit &= ~(1U << 31);
}

void CYB::Engine::Memory::Block::Validate(void) const {
#ifdef DEBUG
	if (FMagicHeader != MAGIC_HEADER || FMagicFooter != MAGIC_FOOTER)
		throw CYB::Exception::Violation(CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK);
#else
	NOP;
#endif
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Block::Splice(const unsigned int ASizeToKeep) noexcept {
	API::Assert::LessThan(ASizeToKeep, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	const auto NewBlockAmount(Size() - ASizeToKeep);
	API::Assert::LessThan<unsigned long long>(sizeof(Block), NewBlockAmount);
	SetSize(ASizeToKeep);
	return *(new (static_cast<byte*>(GetData()) + ASizeToKeep) Block(static_cast<unsigned int>(NewBlockAmount), *this, true));
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Block::EatLeftBlock(void) noexcept {
	API::Assert::False(IsLargeBlock());
	API::Assert::NotEqual<Block*>(LeftBlock(), nullptr);
	const auto NewSize(LeftBlock()->Size() + Size() + sizeof(Block));
	API::Assert::LessThan<unsigned long long>(NewSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	LeftBlock()->SetSize(static_cast<unsigned int>(NewSize));
	return *LeftBlock();
}