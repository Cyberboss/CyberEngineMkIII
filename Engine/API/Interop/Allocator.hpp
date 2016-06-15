//! @file Allocator.hpp Defines CYB::API::Allocator
#pragma once
namespace CYB {
	namespace API {
		class Heap;
		namespace Interop {
			//! @brief Used for allocating objects within and without the engine
			class Allocator {
			private:
				static Allocator* FAllocator;	//!< @brief The allocator singleton
			public:
				Heap& FHeap;	//!< @brief The Heap this allocator uses
			private:
				/*!
					@brief Allocates the object specified by @p AID into FHeap
					@param AID The ID of the Allocatable to allocate
					@param AConstructor A reference to the constructor required by the object
					@return An allocatable pointer which is the base class of the class indicated by AID
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						Dependant on the object being allocated, as well as
						<BR>CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
						<BR>CYB::Exception::Internal::HEAP_CORRUPTION if the integrity of the heap has been compromised
				*/
				virtual Interop::Allocatable* NewObject(const Interop::Allocatable::ID AID, const Interop::EmptyConstructor& AConstructor) = 0;
			protected:
				/*!
					@brief Construct an Allocator
					@param AHeap The Heap the allocator will use
					@par Thread Safety
						Function calls should be syncronized
					@par Exception Safety
						This function does not throw exceptions
				*/
				Allocator(Heap& AHeap) noexcept;
			public:
				/*!
					@brief Retrieve the Allocator singleton
					@return A reference to the Allocator singleton
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static Allocator& GetAllocator(void) noexcept;

				/*!
					@brief Drop in replacement for placement new. Used for code coverage
					@param ALocation An area of memory not nullptr and at least sizof(AType) where AType will be constructed
					@param AArguments Arguments to AType's constructor
					@tparam AType The type to be constructed
					@tparam AArgs The arguments types of AType's constructor
					@return A pointer to the new AType which will be equivalent to ALocation
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						Dependant on AType's constructor
				*/
				template <typename AType, typename... AArgs> static AType* InPlaceAllocation(void* const ALocation, AArgs&&... AArguments);

				/*!
					@brief Allocates the object specified by AAllocatable
					@tparam AAllocatable The type of object to allocate
					@tparam AArgs The arguments types of AAllocatable's constructor
					@param AArguments The arguments of AAllocatable's constructor
					@return An Object specialized on AAllocatable containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						Dependant on the object being allocated, as well as
						<BR>CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
						<BR>CYB::Exception::Internal::HEAP_CORRUPTION if the integrity of the heap has been compromised
				*/
				template <class AAllocatable, typename... AArgs> Object<AAllocatable> NewObject(AArgs&&... AArguments);
				/*!
					@brief Allocates the object specified by AAllocatable
					@tparam AAllocatable The type of object to allocate
					@return An Object specialized on AAllocatable containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						Dependant on the object being allocated, as well as
						<BR>CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the heap does not have the space for the allocation and more system memory cannot be requested
						<BR>CYB::Exception::Internal::HEAP_CORRUPTION if the integrity of the heap has been compromised
				*/
				template <class AAllocatable> Object<AAllocatable> NewObject(void);
			};
		};
	};
	//! @brief See CYB::API::Allocator::GetAllocator
	API::Interop::Allocator& Allocator(void) noexcept;
};