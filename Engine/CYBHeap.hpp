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

			//! @brief See CYB::API::Heap::Alloc
			void* Alloc(const int ASize) final override;
			//! @brief See CYB::API::Heap::Relloc
			void* Realloc(void* const APreviousAllocation, const int ANewSize) final override;
			//! @brief See CYB::API::Heap::Free
			void Free(void* const APreviousAllocation) final override;
		};
	};
};