//! @file CYBBlock.cpp Implements functions from CYB::Engine::Block
#include "CYB.hpp"

static_assert(sizeof(CYB::Engine::Block) ==
#ifdef DEBUG
	32
#else
	16
#endif
	, "Block size has changed, check algorithms");

CYB::Engine::Block::Block(const unsigned int ASpaceAvailable, const Block& ALeftBlock, const bool AFree) :
	FMagicHeader(MAGIC_HEADER),
	FSizeAndFreeBit(InitializeData(ASpaceAvailable, AFree)),
	FOffsetToPreviousBlock(CalculateOffset(ALeftBlock)),
	FMagicFooter(MAGIC_FOOTER)
{
	API::Assert(ASpaceAvailable >= sizeof(Block));
}

unsigned int CYB::Engine::Block::InitializeData(const unsigned int ASize, const bool AFree) {
	const auto DataSize(static_cast<long long>(ASize - sizeof(Block)));
	API::Assert(DataSize >= 0 && DataSize <= std::numeric_limits<int>().max());
	if(AFree)
		return static_cast<unsigned int>(DataSize) | 1U << 31;
	else
		return static_cast<unsigned int>(DataSize) & ~(1U << 31);
}

unsigned int CYB::Engine::Block::CalculateOffset(const Block& ABlock) {
	API::Assert(&ABlock <= this);
	const auto Result(reinterpret_cast<byte*>(this) - reinterpret_cast<const byte*>(&ABlock));
	API::Assert(Result <= std::numeric_limits<unsigned int>().max());
	return static_cast<unsigned int>(Result);
}

unsigned int CYB::Engine::Block::Size(void) const {
	const auto Result(FSizeAndFreeBit & ~(1U << 31));
	API::Assert(Result < static_cast<unsigned int>(std::numeric_limits<int>::max()));
	return Result;
}

bool CYB::Engine::Block::IsFree(void) const {
	return (FSizeAndFreeBit >> 31) & 1;
}

bool CYB::Engine::Block::IsLargeBlock(void) const {
	return IsFree() && Size() == 0;
}

void* CYB::Engine::Block::GetData(void) {
	return this + 1;
}

CYB::Engine::Block& CYB::Engine::Block::FromData(void* const AData) {
	Block& TheBlock(*reinterpret_cast<Block*>(static_cast<byte*>(AData) - sizeof(Block)));
	TheBlock.Validate();
	return TheBlock;
}

CYB::Engine::Block* CYB::Engine::Block::LeftBlock(void) {
	if (FOffsetToPreviousBlock == 0)
		return nullptr;
	return reinterpret_cast<Block*>(reinterpret_cast<byte*>(this) - FOffsetToPreviousBlock);
}

CYB::Engine::Block* CYB::Engine::Block::RightBlock(void) {
	API::Assert(!IsLargeBlock());
	return reinterpret_cast<Block*>(static_cast<byte*>(GetData()) + Size());
}

const CYB::Engine::Block* CYB::Engine::Block::RightBlock(void) const {
	API::Assert(!IsLargeBlock());
	return reinterpret_cast<const Block*>(reinterpret_cast<const byte*>(this) + sizeof(Block) + Size());
}
void CYB::Engine::Block::SetSize(const unsigned int ANewSize) {
	API::Assert(ANewSize < static_cast<unsigned int>(std::numeric_limits<int>::max()));
	FSizeAndFreeBit = (ANewSize | 1U << 31);
}
void CYB::Engine::Block::SetFree(const bool ANewFree) {
	if(ANewFree)
		FSizeAndFreeBit |= 1U << 31;
	else
		FSizeAndFreeBit &= ~(1U << 31);
}

void CYB::Engine::Block::Validate(void) const {
#ifdef DEBUG
	if (FMagicHeader != MAGIC_HEADER || FMagicFooter != MAGIC_FOOTER)
		throw CYB::Exception::Violation(CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK);
#else
	NOP;
#endif
}

CYB::Engine::Block& CYB::Engine::Block::Splice(const unsigned int ASizeToKeep) {
	API::Assert(ASizeToKeep < static_cast<unsigned int>(std::numeric_limits<int>::max()));
	const auto NewBlockAmount(Size() - ASizeToKeep);
	API::Assert(NewBlockAmount > sizeof(Block));
	SetSize(ASizeToKeep);
	return *(new (static_cast<byte*>(GetData()) + ASizeToKeep) Block(static_cast<unsigned int>(NewBlockAmount), *this, true));
}

CYB::Engine::Block& CYB::Engine::Block::EatLeftBlock(void) {
	API::Assert(!IsLargeBlock());
	API::Assert(LeftBlock() != nullptr);
	const auto NewSize(LeftBlock()->Size() + Size() + sizeof(Block));
	API::Assert(NewSize < static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	LeftBlock()->SetSize(static_cast<unsigned int>(NewSize));
	return *LeftBlock();
}