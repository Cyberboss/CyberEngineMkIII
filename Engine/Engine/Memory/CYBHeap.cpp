//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

#ifndef TARGET_OS_WINDOWS
typedef CYB::Platform::Implementation::Posix::__locale_t __locale_t;
#endif

#include <algorithm>

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

void CYB::Engine::Heap::AddToFreeList(Block& ABlock, Block* const APreviousEntry) {
	if (APreviousEntry == nullptr) {
		ABlock.FNextFree = FFreeList;
		FFreeList = &ABlock;
	}
	else {
		ABlock.FNextFree = APreviousEntry->FNextFree;
		APreviousEntry->FNextFree = &ABlock;
	}
}

void CYB::Engine::Heap::RemoveFromFreeList(Block& ABlock, Block* const APreviousEntry) {
	if (APreviousEntry == nullptr)
		FFreeList = FFreeList->FNextFree;
	else
		APreviousEntry->FNextFree = ABlock.FNextFree;
}

void CYB::Engine::Heap::LargeBlockNeedsAtLeast(const int ARequiredNumBytes) {
	if (FLargeBlock->Size() < ARequiredNumBytes) {
		const auto SizeDifference(ARequiredNumBytes - FLargeBlock->Size());
		FCommitSize += SizeDifference;
		Platform::VirtualMemory::Commit(FReservation, FCommitSize);
		FLargeBlock->SetSize(FLargeBlock->Size() + SizeDifference);
	}
}

void CYB::Engine::Heap::MergeBlockIfPossible(Block*& ABlock, Block* ALastFreeListEntry) {
	API::Assert(ABlock->IsFree());

	if (ABlock->LeftBlock() != nullptr && ABlock->LeftBlock()->IsFree()) {
		if (ABlock->IsLargeBlock()) {
			RemoveFromFreeList(*ABlock, ALastFreeListEntry);
			FLargeBlock = &static_cast<LargeBlock*>(ABlock)->EatLeftBlock();
			ABlock = FLargeBlock;
			AddToFreeList(*ABlock, ALastFreeListEntry);
		}
		else {
			const auto NewSize(ABlock->Size() + ABlock->LeftBlock()->Size() + sizeof(Block));

			if (NewSize <= static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
				RemoveFromFreeList(*ABlock, ALastFreeListEntry);
				ABlock = &ABlock->EatLeftBlock();
				AddToFreeList(*ABlock, ALastFreeListEntry);
			}
		}
	}
}

CYB::Engine::Block& CYB::Engine::Heap::AllocImpl(const int ANumBytes, API::LockGuard& ALock) {
	const auto MinimumBlockSize(16);	//Do not splice a block that will have a size smaller than this
	const auto MinimumBlockFootprint(MinimumBlockSize + sizeof(Block));
	Block* LastFreeListEntry(nullptr);
	{
		//First search the free list for the first fit
		Block* CurrentBlock(FFreeList);
		while (CurrentBlock != FLargeBlock) {

			if (CurrentBlock->IsFree()) {

				MergeBlockIfPossible(CurrentBlock, LastFreeListEntry);

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
	auto NewData(AllocImpl(ANumBytes, ALock));
	std::copy(static_cast<byte*>(ABlock.GetData()), static_cast<byte*>(ABlock.GetData()) + ABlock.Size(), static_cast<byte*>(NewData.GetData()));
	Free(ABlock.GetData());
	return Block::FromData(NewData.GetData());
}
void CYB::Engine::Heap::FreeImpl(Block& ABlock, API::LockGuard& ALock) {
	AddToFreeList(ABlock, nullptr);
	ABlock.SetFree(true);
	ALock.Release();
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

