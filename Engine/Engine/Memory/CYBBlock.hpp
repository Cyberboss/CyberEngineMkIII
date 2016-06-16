//! @file CYBBlock.hpp Defines the Block class used by the Heap
#pragma once
namespace CYB {
	namespace Engine {
		namespace Memory {
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
				/*!
					@brief Returns the Size and Free integer given a size and free state
					@param ASize The initial Block size
					@param AFree Whether or not the Block is free
					@return The correctly set Size and Free integer
					@par Thread Safety
						This function requires no thread safety
				*/
				static unsigned int InitializeData(const unsigned int ASize, const bool AFree) noexcept;	// <- It's an unsigned int for a reason dude

				/*!
					@brief Calculate the byte offset to another Block
					@param ABlock The Block to compare with
					@return The offset of this Block with @p ABlock
					@par Thread Safety
						This function requires no thread safety
				*/
				unsigned int CalculateOffset(const Block& ABlock) noexcept;
			public:
				/*!
					@brief Retrieves a reference to a Block given it's data pointer
					@param AData The block's data pointer
					@return A reference to the Block that owns @p AData
					@par Thread Safety
						This function requires no thread safet
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK. Thrown if the Block's magic numbers failed to verify
				*/
				static Block& FromData(void* const AData);
				/*!
					@brief Create a Block. Should be used with operator new in a memory location at least @p ASpaceAvailable large. The Block will own this memory
					@param ASpaceAvailable The amount of memory the block owns. Includes the header
					@param ALeftBlock A pointer to the block behind this one, specifying nullptr indicates this is the first block
					@param AFree Indicates whether or not the Block is free
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				Block(const unsigned int ASpaceAvailable, const Block& ALeftBlock, const bool AFree) noexcept;
				Block(const Block&) = delete;
				/*!
					@brief Get the data portion of the memory owned by this Block
					@return The data portion of the memory owned by this Block
					@par Thread Safety
						This function requires that the object is not modified during this function
				*/
				void* GetData(void) noexcept;

				/*!
					@brief Get the block to the right of this Block
					@return The block to the right of this block. May be nullptr if this is the rightmost block
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				Block* RightBlock(void) noexcept;
				/*!
					@brief Get the block to the right of this Block
					@return The block to the right of this block. May be nullptr if this is the rightmost block
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				const Block* RightBlock(void) const noexcept;
				/*!
					@brief Get the block to the left of this Block
					@return The block to the left of this block. May be nullptr if this is the leftmost block or the block to the left is a LargeBlock
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				Block* LeftBlock(void) noexcept;

				/*!
					@brief Set the Block's size
					@param ANewSize The Block's new size
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				void SetSize(const unsigned int ANewSize) noexcept;
				/*!
					@brief Set the Block's free state
					@param ANewFree The Block's new free state
					@par Thread Safety
						This function requires that the owned memory isn't concurrently accessed during this function
				*/
				void SetFree(const bool ANewFree) noexcept;

				/*!
					@brief Check if this Block is free
					@return true if this Block is free, false otherwise
					@par Thread Safety
						This function requires that the object is not modified during this function
				*/
				bool IsFree(void) const noexcept;
				/*!
					@brief Get the size of the Block
					@return The size of the Block. This will be positive and less than the max of a signed int
					@par Thread Safety
						This function requires that the object is not modified during this function
				*/
				unsigned int Size(void) const noexcept;

				/*!
					@brief Check if this Block is a LargeBlock
					@return true if this Block is a LargeBlock, false otherwise
					@par Thread Safety
						This function requires that the object is not modified during this function
				*/
				bool IsLargeBlock(void) const noexcept;

				/*!
					@brief Checks the validity of a Block. Has no effect outside of DEBUG mode
					@par Thread Safety
						This function requires that the object is not modified during this function
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::INVALID_HEAP_BLOCK. Thrown if the Block's magic numbers failed to verify
				*/
				void Validate(void) const;

				/*!
					@brief Separate this block into two
					@param ASizeToKeep The amount of bytes that should remain in this block
					@return A reference to the Block that was created by the splicing
					@par Thread Safety
						This function requires no thread safety
				*/
				Block& Splice(const unsigned int ASizeToKeep) noexcept;

				/*!
					@brief Merge size and header into the size of the Block to the left. Does not modify free lists
					@par Thread Safety
						This function requires the left Block not be modified during the operation
				*/
				Block& EatLeftBlock(void) noexcept;
			};
		};
	};
};