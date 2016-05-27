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

CYB::Engine::Block& CYB::Engine::Heap::AllocImpl(const int ANumBytes, API::LockGuard& ALock) {
	const auto MinimumBlockSize(16);	//Do not splice a block that will have a size smaller than this
	const auto MinimumBlockFootprint(MinimumBlockSize + sizeof(Block));
	Block* LastFreeListEntry(nullptr);
	{
		//First search the free list for the first fit
		Block* CurrentBlock(FFreeList);
		while (CurrentBlock != FLargeBlock) {

			if (CurrentBlock->Size() >= ANumBytes) {
				//this is the block we're using
				RemoveFromFreeList(*CurrentBlock, LastFreeListEntry);

				//splice it if it's big enough
				if ((CurrentBlock->Size() - ANumBytes) >= MinimumBlockFootprint) {
					auto NewBlock(CurrentBlock->Splice(ANumBytes));
					//and add it to the free list
					AddToFreeList(NewBlock, LastFreeListEntry);

					ALock.Release();
					CurrentBlock->SetSize(ANumBytes);
				}
				ALock.Release();
				CurrentBlock->SetFree(false);
				return *CurrentBlock;
			}

			LastFreeListEntry = CurrentBlock;
			CurrentBlock = CurrentBlock->FNextFree;
		}
	}

	LargeBlockNeedsAtLeast(ANumBytes);

	RemoveFromFreeList(*FLargeBlock, LastFreeListEntry);

	auto& AllocatedBlock(LargeBlock::AllocateBlock(FLargeBlock, ANumBytes));

	AddToFreeList(AllocatedBlock, LastFreeListEntry);

	ALock.Release();

	AllocatedBlock.SetSize(ANumBytes);
	AllocatedBlock.SetFree(false);
	return AllocatedBlock;
}
CYB::Engine::Block& CYB::Engine::Heap::ReallocImpl(Block& ABlock, const int ANumBytes, API::LockGuard& ALock) {
	static_cast<void>(ANumBytes);
	static_cast<void>(ALock);
	return ABlock;
}
void CYB::Engine::Heap::FreeImpl(Block& ABlock, API::LockGuard& ALock) {
	static_cast<void>(ABlock);
	static_cast<void>(ALock);
}

void* CYB::Engine::Heap::Alloc(const int ANumBytes) {
	if (ANumBytes == 0)
		return nullptr;
	API::LockGuard Lock(FMutex);
	return AllocImpl(ANumBytes, Lock).GetData();
}

void* CYB::Engine::Heap::Realloc(void* const APreviousAllocation, const int ANumBytes) {
	if (APreviousAllocation == nullptr)
		return Alloc(ANumBytes);
	
	auto& WorkingBlock(Block::FromData(APreviousAllocation));
	if(WorkingBlock.Size() >= ANumBytes)
		return APreviousAllocation;

	API::LockGuard Lock(FMutex);
	return ReallocImpl(WorkingBlock, ANumBytes, Lock).GetData();
}

void CYB::Engine::Heap::Free(void* const APreviousAllocation) {
	if (APreviousAllocation != nullptr) {
		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
	}
}

