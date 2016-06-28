//! @file CYBHeap.cpp Implements functions from CYB::Engine::Heap
#include "CYB.hpp"

CYB::Engine::Memory::Heap::Heap(const unsigned long long AInitialCommitSize) :
	FReservation(Platform::System::VirtualMemory::Reserve(Parameters::HEAP_RESERVATION_SIZE)),
	FCommitSize(CalculateInitialCommitSize(AInitialCommitSize)),
	FFreeList(nullptr),
	FLocked(false)
{
	Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);

	FLargeBlock = API::Interop::Allocator::InPlaceAllocation<LargeBlock>(&FirstBlock(), FCommitSize - sizeof(LargeBlock), nullptr);
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
	ARequiredNumBytes += sizeof(LargeBlock);
	API::Assert::LessThan(ARequiredNumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	if (FLargeBlock->Size() <= ARequiredNumBytes) {
		const auto SizeDifference(ARequiredNumBytes - FLargeBlock->Size());
		const auto NewCommitSize(FCommitSize + SizeDifference + 1000);
		try {
			Platform::System::VirtualMemory::Commit(FReservation, FCommitSize);
		}catch(CYB::Exception::Internal AException){
			API::Assert::Equal<unsigned int>(AException.FErrorCode, CYB::Exception::Internal::MEMORY_COMMITAL_FAILURE);
			throw CYB::Exception::SystemData(CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE);
		}
		FCommitSize = NewCommitSize;
		FLargeBlock->SetSize(FLargeBlock->Size() + SizeDifference + 1000);
	}
}

void CYB::Engine::Memory::Heap::MergeBlockIfPossible(Block*& ABlock, Block* const ALastFreeListEntry) noexcept {
	API::Assert::True(ABlock->IsFree());

	if (ABlock->LeftBlock() != nullptr && ABlock->LeftBlock()->IsFree()) {
		auto LeftBlock(ABlock->LeftBlock());
		const auto NewSize(ABlock->Size() + LeftBlock->Size() + sizeof(Block));

		if (NewSize <= static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
			RemoveFromFreeList(*ABlock, ALastFreeListEntry);
			ABlock = &ABlock->EatLeftBlock();
			//don't add it to the free list, we're already IN the free list
		}
	}
}

CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::AllocImpl(unsigned int ANumBytes, API::LockGuard& ALock) {
	static_cast<void>(ALock);

	ANumBytes = ANumBytes + (sizeof(void*) - (ANumBytes % sizeof(void*)));	//align

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
			}

			CurrentBlock->SetFree(false);
			return *CurrentBlock;
		}
		else 
			//we can't actually remove it from the free list since we don't know the previous entry which changes if this succeeds, so only do this here
			MergeBlockIfPossible(CurrentBlock, LastFreeListEntry);
		LastFreeListEntry = CurrentBlock;
	}

	LargeBlockNeedsAtLeast(ANumBytes);

	auto& AllocatedBlock(LargeBlock::AllocateBlock(FLargeBlock, ANumBytes));

	AllocatedBlock.SetFree(false);

	return AllocatedBlock;
}
CYB::Engine::Memory::Block& CYB::Engine::Memory::Heap::ReallocImpl(Block& ABlock, unsigned int ANumBytes, API::LockGuard& ALock) {
	ANumBytes = ANumBytes + (sizeof(void*) - (ANumBytes % sizeof(void*)));
	API::Assert::LessThan(ANumBytes, static_cast<unsigned int>(std::numeric_limits<int>::max()));
	auto& RightBlock(ABlock.RightBlock());
	if (!RightBlock.IsLargeBlock()) {
		//only thing we can do without reverse pointers
		auto& NewData(AllocImpl(ANumBytes, ALock));
		ALock.Release();
		std::copy(static_cast<byte*>(ABlock.GetData()), static_cast<byte*>(ABlock.GetData()) + ABlock.Size(), static_cast<byte*>(NewData.GetData()));
		Free(ABlock.GetData());
		return Block::FromData(NewData.GetData());
	}
	else {
		//still pretty decent
		API::Assert::Equal<Block*>(&RightBlock, FLargeBlock);
		const auto Required(ANumBytes - ABlock.Size());
		LargeBlockNeedsAtLeast(Required);

		auto& AllocatedBlock(LargeBlock::AllocateBlock(FLargeBlock, Required));

		API::Assert::Equal(&ABlock, &AllocatedBlock.EatLeftBlock());
		ABlock.SetFree(false);

		return ABlock;
	}
}
void CYB::Engine::Memory::Heap::FreeImpl(Block& ABlock, API::LockGuard& ALock) noexcept(!API::Platform::IsDebug()) {
	static_cast<void>(ALock);
	if (ABlock.LeftBlock() != nullptr && ABlock.LeftBlock()->IsFree()) {
		ABlock.EatLeftBlock().Validate();
	}
	else {
		AddToFreeList(ABlock, nullptr);
		ABlock.SetFree(true);
		ABlock.Validate();
	}
}

void CYB::Engine::Memory::Heap::WalkImpl(API::LockGuard& ALock) const {
	static_cast<void>(ALock);
	long long ExpectedFreeListSize(0);
	for (const Block* CurrentBlock(&FirstBlock()), *PreviousBlock(nullptr); !CurrentBlock->IsLargeBlock(); PreviousBlock = CurrentBlock, CurrentBlock = &CurrentBlock->RightBlock()) {
		CurrentBlock->Validate();
		API::Assert::Equal(static_cast<const Block*>(const_cast<Block*>(CurrentBlock)->LeftBlock()), PreviousBlock);
		if (CurrentBlock->IsFree())
			++ExpectedFreeListSize;
	}
	FLargeBlock->Validate();

	for (auto CurrentBlock(FFreeList); CurrentBlock != nullptr; CurrentBlock = CurrentBlock->FNextFree, --ExpectedFreeListSize)
		API::Assert::True(CurrentBlock->IsFree());

	API::Assert::Equal(ExpectedFreeListSize, 0LL);
}

void CYB::Engine::Memory::Heap::Walk(void) const {
	API::LockGuard Lock(FMutex);
	WalkImpl(Lock);
}

void* CYB::Engine::Memory::Heap::Alloc(const int ANumBytes) {
	if (ANumBytes != 0) {
		if (ANumBytes > 0) {
			if (static_cast<unsigned int>(ANumBytes) <= API::ByteConverters::Megabytes(2047)) {
				API::LockGuard Lock(FMutex);						//alignment
				return AllocImpl(static_cast<unsigned int>(ANumBytes), Lock).GetData();
			}
			throw CYB::Exception::Violation(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
		}
		throw CYB::Exception::Violation(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
	}
	return nullptr;
}

void* CYB::Engine::Memory::Heap::Realloc(void* const APreviousAllocation, const int ANumBytes) {
	if (APreviousAllocation != nullptr) {

		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		if (ANumBytes != 0) {
			if (ANumBytes > 0) {
				if (static_cast<unsigned int>(ANumBytes) <= API::ByteConverters::Megabytes(2047)) {
					if (WorkingBlock.Size() >= static_cast<unsigned int>(ANumBytes))
						return APreviousAllocation;
					else {
						API::LockGuard Lock(FMutex);							
						return ReallocImpl(WorkingBlock, static_cast<unsigned int>(ANumBytes), Lock).GetData();
					}
				}
				throw CYB::Exception::Violation(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
			}
			throw CYB::Exception::Violation(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
		}
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
		return nullptr;
	}
	return Alloc(ANumBytes);
}

void CYB::Engine::Memory::Heap::Free(void* const APreviousAllocation) RELEASE_NOEXCEPT {
	if (APreviousAllocation != nullptr) {
		auto& WorkingBlock(Block::FromData(APreviousAllocation));
		API::LockGuard Lock(FMutex);
		FreeImpl(WorkingBlock, Lock);
	}
}

