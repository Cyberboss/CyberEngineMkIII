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