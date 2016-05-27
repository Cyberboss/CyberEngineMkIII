//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Heap::Heap(const unsigned long long AInitialCommitSize) :
	FReservation(Platform::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE)),
	FCommitSize(CalculateInitialCommitSize(AInitialCommitSize)),
	FLocked(false)
{
	Platform::VirtualMemory::Commit(FReservation, FCommitSize);

	FLargeBlock = new (&FirstBlock()) LargeBlock(FCommitSize);
	FFreeList = FLargeBlock;
}
CYB::Engine::Heap::~Heap() {
	Platform::VirtualMemory::Release(FReservation);
}

unsigned long long CYB::Engine::Heap::CalculateInitialCommitSize(const unsigned long long AInitialCommitSize) {
	const auto MinimumInitialCommit(sizeof(Block) + 1);
	return AInitialCommitSize >= MinimumInitialCommit ? AInitialCommitSize : MinimumInitialCommit;
}

CYB::Engine::Block& CYB::Engine::Heap::FirstBlock(void) {
	return *static_cast<Block*>(FReservation);
}

CYB::Engine::Block& CYB::Engine::Heap::AllocImpl(const int ANumBytes) {
	static_cast<void>(ANumBytes);
	return Block::FromData(nullptr);
}
CYB::Engine::Block& CYB::Engine::Heap::ReallocImpl(Block& ABlock, const int ANumBytes) {
	static_cast<void>(ANumBytes);
	return ABlock;
}
void CYB::Engine::Heap::FreeImpl(Block& ABlock) {
	static_cast<void>(ABlock);
}

void* CYB::Engine::Heap::Alloc(const int ANumBytes) {
	if (ANumBytes == 0)
		return nullptr;
	API::LockGuard Lock(FMutex);
	return AllocImpl(ANumBytes).GetData();
}

void* CYB::Engine::Heap::Realloc(void* const APreviousAllocation, const int ANumBytes) {
	if (APreviousAllocation == nullptr)
		return Alloc(ANumBytes);
	
	auto& WorkingBlock(Block::FromData(APreviousAllocation));
	if(WorkingBlock.Size() >= ANumBytes)
		return APreviousAllocation;

	API::LockGuard Lock(FMutex);
	return ReallocImpl(WorkingBlock, ANumBytes).GetData();
}

void CYB::Engine::Heap::Free(void* const APreviousAllocation) {
	if (APreviousAllocation != nullptr) {
		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock);
	}
}
