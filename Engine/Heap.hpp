//! @file Heap.hpp Defines the Heap interface
namespace CYB {
	namespace API {
		//! @brief An interface which can allocate memory
		class Heap {
		public:
			/*!
				@brief Allocate memory from the heap for use
				@param ASize The amount of bytes to allocate
				@return The beginning of an allocated memory region with size at least @p ASize
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
			*/
			virtual void* Alloc(const unsigned int ASize) = 0;
			/*!
				@brief Allocate memory from the heap for use while preserving previous data. Passing a valid @p APreviousAllocation with @p ANewSize of zero is equivalent to calling Free(@p APreviousAllocation)
				@param APreviousAllocation A pointer previously returned from a call to Alloc or Realloc of the same Heap object
				@param ANewSize The amount of bytes to allocate
				@return The beginning of an allocated memory region with size at least @p ANewSize and the data from @p APreviousAllocation
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
			*/
			virtual void* Realloc(void* const APreviousAllocation, const unsigned int ANewSize) = 0;
			/*!
				@brief Return memory to the heap. Data in allocated range will be lost
				@param APreviousAllocation A pointer previously returned from a call to Alloc or Realloc of the same Heap object
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			virtual void Free(void* const APreviousAllocation) = 0;
		};
	};
};