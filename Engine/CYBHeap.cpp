//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Heap::Heap(const unsigned long long AInitialCommitSize) :
	FReservation(Platform::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE)),
	FCommitSize(CalculateInitialCommitSize(AInitialCommitSize)),
	FLocked(false)
{
	Platform::VirtualMemory::Commit(FReservation, FCommitSize);

	new (&FirstBlock()) LargeBlock(FCommitSize);
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

void* CYB::Engine::Heap::Alloc(const int ANumBytes) {
	//! @todo implement
	static_cast<void>(ANumBytes);
	return nullptr;
}

void* CYB::Engine::Heap::Realloc(void* const APreviousAllocation, const int ANumBytes) {
	//! @todo implement
	static_cast<void>(APreviousAllocation);
	static_cast<void>(ANumBytes);
	return nullptr;
}

void CYB::Engine::Heap::Free(void* const APreviousAllocation) {
	//! @todo implement
	static_cast<void>(APreviousAllocation);
}

