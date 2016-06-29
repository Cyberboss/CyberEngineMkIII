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
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::NEGATIVE_HEAP_ALLOCATION. Thrown if an allocation was attempted with a negative size value
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::MEMORY_COMMITAL_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				@attention Allocations above 2047MB are unsupported
			*/
			virtual void* Alloc(const int ASize) = 0;
			/*!
				@brief Allocate memory from the heap for use while preserving previous data. Passing a valid @p APreviousAllocation with @p ANewSize of zero is equivalent to calling Free(@p APreviousAllocation)
				@param APreviousAllocation A pointer previously returned from a call to Alloc or Realloc of the same Heap object
				@param ANewSize The amount of bytes to allocate or zero to free an allocation from @p APreviousAllocation. Must be less than 2047MB
				@return The beginning of an allocated memory region with size at least @p ANewSize and the data from @p APreviousAllocation. Or, if @p ANewSize was less than or equal to zero, nullptr
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::NEGATIVE_HEAP_ALLOCATION. Thrown if an allocation was attempted with a negative size value
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::ErrorCode::UNSUPPORTED_ALLOCATION_AMOUNT if ANewSize is greater than or equal to 2047MB
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::HEAP_ALLOCATION_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
			*/
			virtual void* Realloc(void* const APreviousAllocation, const int ANewSize) = 0;
			/*!
				@brief Return memory to the heap. Data in allocated range will be lost
				@param APreviousAllocation A pointer previously returned from a call to Alloc or Realloc of the same Heap object
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Free(void* const APreviousAllocation) noexcept = 0;
		};
	};
};