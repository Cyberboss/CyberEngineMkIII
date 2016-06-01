//! @file CYBBlock.hpp Defines the LargeBlock class used by the Heap
#pragma once
namespace CYB {
	namespace Engine {
		//! @brief Used to identify the end of a Heap
		class LargeBlock : public Block {
		private:
#ifdef DEBUG	//Block debug headers
			unsigned long long FMagicHeader; //!< @brief A guard against memory corruption
#endif
			unsigned long long FRemainingSize; //!< @brief The amount of space the block has available
#ifdef DEBUG
			unsigned long long FMagicFooter; //!< @brief A guard against memory corruption
#endif
		public:
			/*!
				@brief Allocate a portion of the LargeBlock's owned data to create a regular block
				@param[out] ALargeBlock A reference to the pointer pointing to the LargeBlock from which the new Block will be allocated. Will be changed to the LargeBlock's new location upon return
				@param ANewBlockSize The size of the Block to be allocated
				@return A new Block with the correct size and free state
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			static Block& AllocateBlock(LargeBlock*& ALargeBlock, const unsigned int ANewBlockSize);

			/*!
				@brief Initialization constructor. Should be used with operator new in a memory location at least @p ASpaceAvailable large that extends to the end of a reservation. The LargeBlock will own this memory
				@param ASpaceAvailable The amount of space the large block is using. Disincludes the header
				@param ALeftBlock A pointer to the Block to the left of the LargeBlock, or nullptr if the LargeBlock is the only Block
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			LargeBlock(const unsigned long long ASpaceAvailable, Block* const ALeftBlock);

			//! @brief See CYB::Engine::Block::Validate. This version must be used when referring to large blocks
			void Validate(void) const;

			/*!
				@brief Merge size and header into the size of the Block to the left. Does not modify free lists. Must be used if Block is a LargeBlock
				@par Thread Safety
					This function requires the left Block not be modified during the operation
				@par Exception Safety
					This function does not throw exceptions
			*/
			LargeBlock& EatLeftBlock(void);

			//! @brief See CYB::Engine::Block::Size. This version must be used when referring to large blocks
			unsigned long long Size(void) const;

			//! @brief See CYB::Engine::Block::SetSize. This version must be used when referring to large blocks
			void SetSize(const unsigned long long ANewSize);
		};
	};
}; 
