//! @file CYBHeap.hpp Defines the Heap implentation
#pragma once
namespace CYB {
	namespace Engine {
		//! @brief A memory pool manager and allocator
		class Heap : public API::Heap {
		private:
			void* FReservation; //!< @brief The VirtualMemory mapping owned by the heap, also a pointer to the first block
			unsigned long long FCommitSize; //!< @brief The amount of memory currently available in the Heap
			bool FLocked; //!< @brief Whether or not the Heap is currently locked

			Block* FFreeList; //!< @brief The first block in the linked free list
			LargeBlock* FLargeBlock; //!< @brief The block that extends to the end of the free list

			Platform::Mutex FMutex;	//!< @brief The lock for the Heap
		private:
			/*!
				@brief A small max comparison of @p AInitialCommitSize and sizeof(Block) + 1
				@param AInitialCommitSize The wanted initial commit size
				@return The minimum valid initial commit size
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			static unsigned long long CalculateInitialCommitSize(const unsigned long long AInitialCommitSize);

			/*!
				@brief Get a reference to the first block in the reservation
				@return A reference to the first block in the reservation
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			Block& FirstBlock(void);
			/*!
				@brief Get a reference to the first block in the reservation
				@return A reference to the first block in the reservation
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			const Block& FirstBlock(void) const;

			/*!
				@brief Adds a Block to the free list after @p APreviousEntry while performing all the checks and reassignments
				@param ABlock The Block to add to the free list
				@param APreviousEntry The Block that will preceed @p ABlock in the free list
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					This function does not throw exceptions
			*/
			void AddToFreeList(Block& ABlock, Block* const APreviousEntry);
			/*!
				@brief Removes a Block from the free list after @p APreviousEntry while performing all the checks and reassignments
				@param ABlock The Block to remove from the free list
				@param APreviousEntry The Block that preceeds @p ABlock in the free list
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					This function does not throw exceptions
			*/
			void RemoveFromFreeList(Block& ABlock, Block* const APreviousEntry);

			/*!
				@brief Ensures that FLargeBlock has at least @p ARequiredNumBytes of Size by committing more memory if necessary
				@param ARequiredNumBytes The required number of bytes from the LargeBlock
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					This function does not throw exceptions
			*/
			void LargeBlockNeedsAtLeast(const unsigned int ARequiredNumBytes);

			/*!
				@brief Coalesces Blocks to the left of @p ABlock and updates the free list
				@param ABlock A free Block
				@param ALastFreeListEntry The Block preceeding @p ABlock in the free list
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					This function does not throw exceptions
			*/
			void MergeBlockIfPossible(Block*& ABlock, Block* ALastFreeListEntry);

			/*! 
				@brief Allocates a Block
				@param ANumBytes The minimum number of bytes available in the returned Block
				@param ALock A reference to the current lock on FMutex that will be released by the time the function returns
				@return An unused Block that isn't free and is is at least @p ANumBytes in size
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the memory could not be committed
			*/
			Block& AllocImpl(const unsigned int ANumBytes, API::LockGuard& ALock);
			/*! 
				@brief Reallocates a Block
				@param ABlock A reference to the Block being worked on
				@param ANumBytes The minimum number of bytes available in the returned Block
				@param ALock A reference to the current lock on FMutex that will be released by the time the function returns
				@return An unused Block that isn't free, is at least @p ANumBytes in size, and contains all previous data from @p ABlock
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					CYB::Exception::Violation::INVALID_HEAP_BLOCK if the Block's magic numbers failed to verify
					<BR>CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the memory could not be committed
			*/
			Block& ReallocImpl(Block& ABlock, const unsigned int ANumBytes, API::LockGuard& ALock);
			/*!
				@brief Frees a Block
				@param ABlock A reference to the Block being worked on
				@param ALock A reference to the current lock on FMutex that will be released by the time the function returns
				@par Thread Safety
					This function requires that FMutex is locked
				@par Exception Safety
					CYB::Exception::Violation::INVALID_HEAP_BLOCK if the Block's magic numbers failed to verify
			*/
			void FreeImpl(Block& ABlock, API::LockGuard& ALock);
		public:
			/*!
				@brief Create a Heap
				@param AInitialCommitSize The amount of memory to initalize the heap with, must be smaller than @p AReservationSize
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the memory could not be committed
			*/
			Heap(const unsigned long long AInitialCommitSize);
			Heap(const Heap&) = delete;
			Heap(Heap&& AMove);
			Heap& operator=(Heap&& AMove);
			/*!
				@brief Destroys a heap, invalidating any memory allocated from it
				@par WARNING
					Potential hard crash due to possiblity of uncaught exception: CYB::Exception::SystemData::MEMORY_RELEASE_FAILURE
			*/
			~Heap();

			/*!
				@brief Lock the Heap, preventing reads and writes to the owned memory
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE if the memory protections could not be changed
			*/
			void Lock(void);
			/*!
				@brief Unlock the Heap, allowing reads and writes to the owned memory
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE if the memory protections could not be changed
			*/
			void Unlock(void);
			/*!
				@brief Check and see if the Heap is currently locked
				@return true if the Heap is Locked, false otherwise
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			bool Locked(void) const;

			/*!
				@brief Retrieve the number of allocations over the Heap's lifetime
				@return The number of allocations over the Heap's lifetime
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			unsigned long long TotalNumberOfAllocations(void) const;
			/*!
				@brief Retrieve the current of allocations in the heap. Implicitly unlocks the Heap for the duration of the function
				@return The number of current allocations in the heap
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::Internal::HEAP_CORRUPTION if the integrity of the heap has been compromised
			*/
			unsigned long long CurrentNumberOfAllocations(void) const;
			/*!
				@brief Retrieve the amount of memory currently committed by the heap
				@return The amount of memory currently committed by the heap
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			unsigned long long CurrentAmountOfMemoryCommitted(void) const;

			/*!
				@brief Walks the heap and throws if an error is detected
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::Violation::INVALID_HEAP_BLOCK if a Block's magic numbers failed to verify
			*/
			void Walk(void) const;

			//! @brief See CYB::API::Heap::Alloc
			void* Alloc(const int ASize) final override;
			//! @brief See CYB::API::Heap::Relloc
			void* Realloc(void* const APreviousAllocation, const int ANewSize) final override;
			//! @brief See CYB::API::Heap::Free
			void Free(void* const APreviousAllocation) final override;
		};
	};
};