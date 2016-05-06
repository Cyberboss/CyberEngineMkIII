//! @file CYBBlock.hpp Defines the Block class used by the Heap
#pragma once
namespace CYB {
	namespace Engine {
		//! @brief A unit of memory allocation
		class Block {
		protected:
			enum : unsigned long long {
				MAGIC_HEADER = 0x0123456789ABCDEF,
				MAGIC_FOOTER = 0xFEDCBA9876543210,
			};
		private:
#ifdef DEBUG	//Block debug headers
			unsigned long long FMagicHeader; //!< @brief A guard against memory corruption
#endif
			Block* FNextFree;	//!< @brief The next block in the free list
			unsigned int FSizeAndFreeBit, //!< @brief The size of this block and it's free bit
				FOffsetToPreviousBlock;	//!< @brief The offset from the block behind it
#ifdef DEBUG
			unsigned long long FMagicFooter; //!< @brief A guard against memory corruption
#endif
		private:
			static unsigned int InitializeData(const int ASize, const bool AFree);

			unsigned int CalculateOffset(const Block& ABlock);
		public:
			/*!
				@brief Create a Block. Should be used with operator new in a memory location at least @p ASpaceAvailable large. The Block will own this memory
				@param ASpaceAvailable The amount of memory the block owns
				@param ALeftBlock A pointer to the block behind this one, specifying nullptr indicates this is the first block
				@param AFree Indicates whether or not the Block is free
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			Block(const int ASpaceAvailable, const Block& ALeftBlock, const bool AFree);
		};
	};
};