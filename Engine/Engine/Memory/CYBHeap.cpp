//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Memory::Heap::Heap(const unsigned long long AInitialCommitSize) :
	FReservation(Platform::System::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE)),
	FCommitSize(CalculateInitialCommitSize(AInitialCommitSize)),
	FLocked(false)
{
	Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);

	FLargeBlock = API::Interop::Allocator::InPlaceAllocation<LargeBlock>(&FirstBlock(), FCommitSize, nullptr);
	FFreeList = FLargeBlock;
}

CYB::Engine::Memory::Heap::~Heap() {
	if(FReservation != nullptr)
		Platform::System::VirtualMemory::Release(FReservation);
}

unsigned long long CYB::Engine::Memory::Heap::CalculateInitialCommitSize(const unsigned long long AInitialCommitSize) noexcept {
	const auto MinimumInitialCommit(sizeof(Block) + 1);
	return AInitialCommitSize >= MinimumInitialCommit ? AInitialCommitSize : MinimumInitialCommit;
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::FirstBlock(void) noexcept {
	return *static_cast<Block*>(FReservation);
}

const CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::FirstBlock(void) const noexcept {
	return *static_cast<Block*>(FReservation);
}

void CYB::Engine::Memory::Heap::AddToFreeList(Block& ABlock, Block* const APreviousEntry) noexcept {
	if (APreviousEntry == nullptr) {
		ABlock.FNextFree = FFreeList;
		FFreeList = &ABlock;
	}
	else {
		ABlock.FNextFree = APreviousEntry->FNextFree;
		APreviousEntry->FNextFree = &ABlock;
	}
}

void CYB::Engine::Memory::Heap::RemoveFromFreeList(Block& ABlock, Block* const APreviousEntry) noexcept {
	if (APreviousEntry == nullptr)
		FFreeList = FFreeList->FNextFree;
	else
		APreviousEntry->FNextFree = ABlock.FNextFree;
}

void CYB::Engine::Memory::Heap::LargeBlockNeedsAtLeast(const unsigned int ARequiredNumBytes) {
	API::Assert::LessThan(ARequiredNumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	if (FLargeBlock->Size() < ARequiredNumBytes) {
		const auto SizeDifference(ARequiredNumBytes - FLargeBlock->Size());
		FCommitSize += SizeDifference;
		Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);
		FLargeBlock->SetSize(FLargeBlock->Size() + SizeDifference);
	}
}

void CYB::Engine::Memory::Heap::MergeBlockIfPossible(Block*& ABlock, Block* ALastFreeListEntry) noexcept {
	API::Assert::True(ABlock->IsFree());

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

CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::AllocImpl(const unsigned int ANumBytes, API::LockGuard& ALock) {
	API::Assert::LessThan(ANumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
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
						auto& NewBlock(CurrentBlock->Splice(ANumBytes));
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
CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::ReallocImpl(Block& ABlock, const unsigned int ANumBytes, API::LockGuard& ALock) {
	API::Assert::LessThan(ANumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	auto& NewData(AllocImpl(ANumBytes, ALock));
	std::copy(static_cast<byte*>(ABlock.GetData()), static_cast<byte*>(ABlock.GetData()) + ABlock.Size(), static_cast<byte*>(NewData.GetData()));
	Free(ABlock.GetData());
	return Block::FromData(NewData.GetData());
}
void CYB::Engine::Memory::Heap::FreeImpl(Block& ABlock, API::LockGuard& ALock) {
	AddToFreeList(ABlock, nullptr);
	ABlock.SetFree(true);
	ALock.Release();
}

void CYB::Engine::Memory::Heap::Walk(void) const {
	API::LockGuard Lock(FMutex);
	for (auto CurrentBlock(&FirstBlock()); CurrentBlock != FLargeBlock; CurrentBlock = &CurrentBlock->RightBlock())
		CurrentBlock->Validate();
	FLargeBlock->Validate();
}

void* CYB::Engine::Memory::Heap::Alloc(const int ANumBytes) {
	if (ANumBytes == 0)
		return nullptr;
	else if (ANumBytes < 0)
		throw CYB::Exception::Violation(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
	API::LockGuard Lock(FMutex);						//alignment
	return AllocImpl(static_cast<unsigned int>(ANumBytes + (sizeof(void*) - (ANumBytes % sizeof(void*)))), Lock).GetData();
}

void* CYB::Engine::Memory::Heap::Realloc(void* const APreviousAllocation, const int ANumBytes) {
	if (APreviousAllocation == nullptr)
		return Alloc(ANumBytes);
	
	auto& WorkingBlock(Block::FromData(APreviousAllocation));
	if (ANumBytes == 0) {
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
		return nullptr;
	}
	else if (ANumBytes < 0)
		throw CYB::Exception::Violation(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
	else if(WorkingBlock.Size() >= static_cast<unsigned int>(ANumBytes))
		return APreviousAllocation;

	API::LockGuard Lock(FMutex);								//alignment
	return ReallocImpl(WorkingBlock, static_cast<unsigned int>(ANumBytes + (sizeof(void*) - (ANumBytes % sizeof(void*)))), Lock).GetData();
}

void CYB::Engine::Memory::Heap::Free(void* const APreviousAllocation) {
	if (APreviousAllocation != nullptr) {
		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
	}
}

