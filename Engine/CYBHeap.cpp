//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Heap::Heap(const unsigned long long AInitialCommitSize):
	FReservation(Platform::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE))
{
	const auto MinimumInitialCommit(sizeof(Block) + 1);
	Platform::VirtualMemory::Commit(FReservation, AInitialCommitSize >= MinimumInitialCommit ? AInitialCommitSize : MinimumInitialCommit);
}
CYB::Engine::Heap::~Heap() {
	Platform::VirtualMemory::Release(FReservation);
}

void* CYB::Engine::Heap::Alloc(const unsigned int ANumBytes) {
	//! @todo implement
	static_cast<void>(ANumBytes);
	return nullptr;
}

void* CYB::Engine::Heap::Realloc(void* const APreviousAllocation, const unsigned int ANumBytes) {
	//! @todo implement
	static_cast<void>(APreviousAllocation);
	static_cast<void>(ANumBytes);
	return nullptr;
}

void CYB::Engine::Heap::Free(void* const APreviousAllocation) {
	//! @todo implement
	static_cast<void>(APreviousAllocation);
}

