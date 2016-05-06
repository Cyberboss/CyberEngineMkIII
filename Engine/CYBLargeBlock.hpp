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
			LargeBlock* FLargeBlockToTheRight; //!< @brief Pointer to a large block in a higher memory location in the current reservation
			unsigned long long FRemainingSize; //!< @brief The amount of space the block has available
#ifdef DEBUG
			unsigned long long FMagicFooter; //!< @brief A guard against memory corruption
#endif
		public:
			/*!
				@brief Initialization constructor. Should be used with operator new in a memory location at least @p ASpaceAvailable large that extends to the end of a reservation. The LargeBlock will own this memory
				@param ASpaceAvailable The amount of space the large block is using
				@par Thread Safety
					This function requires that the owned memory isn't concurrently accessed during this function
				@par Exception Safety
					This function does not throw exceptions
			*/
			LargeBlock(const unsigned long long ASpaceAvailable);
		};
	};
}; 
