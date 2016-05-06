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

#ifdef DEBUG	//Block debug headers
		private:
			unsigned long long FMagicHeader; //!< @brief A guard against memory corruption
#endif
		public:
			Block* FNextFree;	//!< @brief The next block in the free list
		private:
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
			
			/*!
				@brief Get the data portion of the memory owned by this Block
				@return The data portion of the memory owned by this Block
				@par Thread Safety
					This function requires that the object is not modified during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			void* GetData(void);

			/*!
				@brief Get the block to the right of this Block
				@return The block to the right of this block. May be nullptr if this is the rightmost block
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			Block* RightBlock(void);
			/*!
				@brief Get the block to the left of this Block
				@return The block to the left of this block. May be nullptr if this is the leftmost block or the block to the left is a LargeBlock
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			Block* LeftBlock(void);

			/*!
				@brief Set the Block's size
				@param ANewSize The Block's new size
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			void SetSize(const int ANewSize);
			/*!
				@brief Set the Block's free state
				@param ANewFree The Block's new free state
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			void SetFree(const bool ANewFree);

			/*!
				@brief Check if this Block is free
				@return true if this Block is free, false otherwise
				@par Thread Safety
					This function requires that the object is not modified during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			bool IsFree(void) const;
			/*!
				@brief Get the size of the Block
				@return The size of the Block. This will be positive
				@par Thread Safety
					This function requires that the object is not modified during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			int Size(void) const;

			/*!
				@brief Check if this Block is a LargeBlock			
				@return true if this Block is a LargeBlock, false otherwise
				@par Thread Safety
					This function requires that the object is not modified during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			bool IsLargeBlock(void) const;
		};
	};
};