//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Memory::Heap::Heap(const unsigned long long AInitialCommitSize) :
	FReservation(Platform::System::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE)),
	FCommitSize(CalculateInitialCommitSize(AInitialCommitSize)),
	FFreeList(nullptr),
	FLocked(false)
{
	Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);

	FLargeBlock = API::Interop::Allocator::InPlaceAllocation<LargeBlock>(&FirstBlock(), FCommitSize, nullptr);
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

void CYB::Engine::Memory::Heap::LargeBlockNeedsAtLeast(unsigned int ARequiredNumBytes) {
	ARequiredNumBytes += sizeof(Block);
	API::Assert::LessThan(ARequiredNumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	if (FLargeBlock->Size() < ARequiredNumBytes) {
		const auto SizeDifference(ARequiredNumBytes - FLargeBlock->Size());
		FCommitSize += ARequiredNumBytes + 1000 - SizeDifference;
		Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);
		FLargeBlock->SetSize(FLargeBlock->Size() + SizeDifference + 1000);
	}
}

void CYB::Engine::Memory::Heap::MergeBlockIfPossible(Block*& ABlock, Block* const ALastFreeListEntry) noexcept {
	API::Assert::True(ABlock->IsFree());

	if (ABlock->LeftBlock() != nullptr && ABlock->LeftBlock()->IsFree()) {
		if (ABlock->IsLargeBlock()) {
			RemoveFromFreeList(*ABlock, ALastFreeListEntry);
			FLargeBlock = &static_cast<LargeBlock*>(ABlock)->EatLeftBlock();
			ABlock = FLargeBlock;
		}
		else {
			const auto NewSize(ABlock->Size() + ABlock->LeftBlock()->Size() + sizeof(Block));

			if (NewSize <= static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
				RemoveFromFreeList(*ABlock, ALastFreeListEntry);
				ABlock = &ABlock->EatLeftBlock();
				//don't add it to the free list, we're already IN the free list
			}
		}
	}
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::AllocImpl(const unsigned int ANumBytes, API::LockGuard& ALock) {
	API::Assert::LessThan(ANumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	const auto MinimumBlockSize(16);	//Do not splice a block that will have a size smaller than this
	const auto MinimumBlockFootprint(MinimumBlockSize + sizeof(Block));

	//First search the free list for the first fit
	for (Block* CurrentBlock(FFreeList), *LastFreeListEntry(nullptr); CurrentBlock != nullptr; CurrentBlock = CurrentBlock->FNextFree) {

		API::Assert::True(CurrentBlock->IsFree());

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
		}else
			//we can't actually remove it from the free list since we don't know the previous entry which changes if this succeeds, so only do this here
			MergeBlockIfPossible(CurrentBlock, LastFreeListEntry);

		LastFreeListEntry = CurrentBlock;
	}

	LargeBlockNeedsAtLeast(ANumBytes);

	auto& AllocatedBlock(LargeBlock::AllocateBlock(FLargeBlock, ANumBytes));

	ALock.Release();

	AllocatedBlock.SetSize(ANumBytes);
	AllocatedBlock.SetFree(false);

	return AllocatedBlock;
}
CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::ReallocImpl(Block& ABlock, const unsigned int ANumBytes, API::LockGuard& ALock) {
	API::Assert::LessThan(ANumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	auto& RightBlock(ABlock.RightBlock());
	if (!RightBlock.IsLargeBlock()) {
		const auto CombinedSize(ABlock.Size() + RightBlock.Size() + sizeof(Block));				//can't splice it, we don't have the previous free list entry
		if (RightBlock.IsFree() && &RightBlock == ABlock.FNextFree && CombinedSize >= ANumBytes && CombinedSize <= static_cast<unsigned int>(std::numeric_limits<int>::max())) {
			//jackpot
			ABlock.FNextFree = RightBlock.FNextFree;
			ALock.Release();

			API::Assert::Equal(&ABlock, &RightBlock.EatLeftBlock());

			return ABlock;
		}
		else {
			//only thing we can do without reverse pointers
			auto& NewData(AllocImpl(ANumBytes, ALock));
			std::copy(static_cast<byte*>(ABlock.GetData()), static_cast<byte*>(ABlock.GetData()) + ABlock.Size(), static_cast<byte*>(NewData.GetData()));
			Free(ABlock.GetData());
			return Block::FromData(NewData.GetData());
		}
	}
	else {
		//still pretty decent
		API::Assert::Equal<Block*>(&RightBlock, FLargeBlock);
		const auto Required(ANumBytes - ABlock.Size());
		LargeBlockNeedsAtLeast(Required);

		auto& AllocatedBlock(LargeBlock::AllocateBlock(FLargeBlock, Required));

		ALock.Release();

		API::Assert::Equal(&ABlock, &AllocatedBlock.EatLeftBlock());
		ABlock.SetFree(false);

		return ABlock;
	}
}
void CYB::Engine::Memory::Heap::FreeImpl(Block& ABlock, API::LockGuard& ALock) RELEASE_NOEXCEPT {
	AddToFreeList(ABlock, nullptr);
	ABlock.SetFree(true);
	ALock.Release();
}

void CYB::Engine::Memory::Heap::Walk(void) const {
	API::LockGuard Lock(FMutex);
	long long ExpectedFreeListSize(0);
	for (auto CurrentBlock(&FirstBlock()); !CurrentBlock->IsLargeBlock(); CurrentBlock = &CurrentBlock->RightBlock()) {
		CurrentBlock->Validate();
		if (CurrentBlock->IsFree())
			++ExpectedFreeListSize;
	}
	FLargeBlock->Validate();

	for (auto CurrentBlock(FFreeList); CurrentBlock != nullptr; CurrentBlock = CurrentBlock->FNextFree, --ExpectedFreeListSize)
		if (!CurrentBlock->IsFree())
			throw CYB::Exception::Internal(CYB::Exception::Internal::INVALID_HEAP_FREE_LIST);

	if(ExpectedFreeListSize != 0)
		throw CYB::Exception::Internal(CYB::Exception::Internal::INVALID_HEAP_FREE_LIST);
}

void* CYB::Engine::Memory::Heap::Alloc(const int ANumBytes) {
	if (ANumBytes == 0)
		return nullptr;
	else if (ANumBytes < 0)
		throw CYB::Exception::Violation(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
	else if (static_cast<unsigned int>(ANumBytes) > API::ByteConverters::Megabytes(2047))
		throw CYB::Exception::Violation(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
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
	else if (static_cast<unsigned int>(ANumBytes) > API::ByteConverters::Megabytes(2047))
		throw CYB::Exception::Violation(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
	else if(WorkingBlock.Size() >= static_cast<unsigned int>(ANumBytes))
		return APreviousAllocation;

	API::LockGuard Lock(FMutex);								//alignment
	return ReallocImpl(WorkingBlock, static_cast<unsigned int>(ANumBytes + (sizeof(void*) - (ANumBytes % sizeof(void*)))), Lock).GetData();
}

void CYB::Engine::Memory::Heap::Free(void* const APreviousAllocation) RELEASE_NOEXCEPT {
	if (APreviousAllocation != nullptr) {
		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
	}
}

