//! @file CYBBlock.hpp Defines the LargeBlock class used by the Heap
#pragma once
namespace CYB {
	namespace Engine {
		//! @brief Used to identify the end of a Heap
		class LargeBlock : public Block {
#ifdef DEBUG	//Block debug headers
		private:
			unsigned long long FMagicHeader; //!< @brief A guard against memory corruption
#endif
		public:
			LargeBlock* FLargeBlockToTheRight; //!< @brief Pointer to a large block in a higher memory location in the current reservation
		private:
			unsigned long long FRemainingSize; //!< @brief The amount of space the block has available
#ifdef DEBUG
			unsigned long long FMagicFooter; //!< @brief A guard against memory corruption
#endif
		private:
			/*!
				@brief Allocation constructor. Should be used with operator new in a memory location at least @p ASpaceAvailable large. The LargeBlock will own this memory
				@param ASpaceAvailable The amount of space the large block is using
				@param ALargeBlockToTheRight The allocated LargeBlock's value of FLargeBlockToTheRight
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			LargeBlock(const unsigned long long ASpaceAvailable, LargeBlock* ALargeBlockToTheRight);
		public:
			/*!
				@brief Allocate a portion of the LargeBlock's owned data to create a regular block
				@param[out] ALargeBlock A reference to the pointer pointing to the LargeBlock from which the new Block will be allocated. Will be changed to the LargeBlock's new location upon return
				@param ANewBlockSize The size of the Block to be allocated
				@return A new Block with the correct size and free state if it succeeded. Nullptr will be return in the other case of not having enough space in @p ALargeBlock
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			static Block& AllocateBlock(LargeBlock*& ALargeBlock, const int ANewBlockSize);

			/*!
				@brief Initialization constructor. Should be used with operator new in a memory location at least @p ASpaceAvailable large that extends to the end of a reservation. The LargeBlock will own this memory
				@param ASpaceAvailable The amount of space the large block is using
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			LargeBlock(const unsigned long long ASpaceAvailable);

			//! @brief See CYB::Engine::Block::Validate. This version must be used when referring to large blocks
			void Validate(void) const;
		};
	};
}; 
