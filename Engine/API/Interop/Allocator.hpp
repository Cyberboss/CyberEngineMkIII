//! @file Allocator.hpp Defines CYB::API::Allocator
#pragma once
namespace CYB {
	namespace API {
		class Heap;
		namespace Interop {
			//! @brief Used for allocating objects within and without the engine
			class Allocator {
				ENABLE_TEST_HOOKS
			public:
				Heap& FHeap;	//!< @brief The Heap this allocator uses
			private:
				/*!
					@brief Allocates the object specified by @p AID into FHeap
					@param AID The ID of the Allocatable to allocate
					@param AConstructor A reference to the constructor required by the object
					@return An Allocatable pointer which is the base class of the class indicated by AID
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AID is unknown to the engine.
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_INTEROP_CONSTRUCTOR. Thrown if @p AConstructor is malformed
				*/
				virtual void* InteropAllocation(const Allocatable::ID AID, Constructor<void>& AConstructor) = 0;
				/*!
					@brief Drop in replacement for placement new with successful abstraction checking
					@tparam AType The type to be constructed
					@tparam AArgs The arguments types of AType's constructor
					@param ALocation An area of memory not nullptr and at least sizof(AType) where AType will be constructed
					@param AIgnored Used for overloading
					@param AArguments Arguments to AType's constructor
					@return A pointer to the new AType which will be equivalent to ALocation
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
				*/
				template <typename AType, typename... AArgs> static AType* InPlaceAllocation(void* const ALocation, std::false_type AIgnored, AArgs&&... AArguments);
				/*!
					@brief Drop in replacement for placement new with failed abstraction checking. Calls HCF
					@tparam AType Ignored
					@tparam AArgs Ignored
					@param ALocation Ignored
					@param AIgnored Ignored
					@param AArguments Ignored
					@return A pointer to the new AType which will be equivalent to ALocation
					@par Thread Safety
						This function requires no thread safety
				*/
				template <typename AType, typename... AArgs> static AType* InPlaceAllocation(void* const ALocation, std::true_type AIgnored, AArgs&&... AArguments) noexcept;
			protected:
				/*!
					@brief Construct an Allocator
					@param AHeap The Heap the allocator will use
					@par Thread Safety
						Function calls should be syncronized
				*/
				Allocator(Heap& AHeap) noexcept;
			public:
				/*!
					@brief Drop in replacement for placement new. Used for code coverage
					@param ALocation An area of memory not nullptr and at least sizof(AType) where AType will be constructed
					@param AArguments Arguments to AType's constructor
					@tparam AType The type to be constructed
					@tparam AArgs The arguments types of AType's constructor
					@return A pointer to the new AType which will be equivalent to ALocation
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
				*/
				template <typename AType, typename... AArgs> static AType* InPlaceAllocation(void* const ALocation, AArgs&&... AArguments);

				/*!
					@brief Allocates the Object specified by AAllocatable
					@tparam AAllocatable The type of object to allocate
					@tparam AConstructor The constructor for AAllocatable to use. Defaults to AAllocatable::Constructor
					@tparam AArgs The arguments types of AAllocatable's constructor
					@param AArguments The arguments of AAllocatable's constructor
					@return An Object specialized on AAllocatable containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject, class AConstructor = AObject::Constructor, typename... AArgs> Object<AObject> NewObject(AArgs&&... AArguments);
				/*!
					@brief Allocates the Object specified by AAllocatable
					@tparam AAllocatable The type of object to allocate
					@return An Object specialized on AAllocatable containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject> Object<AObject> NewObject(void);

				/*!
					@brief Copys an Object of type AAllocatable
					@tparam AAllocatable The type of object to copy
					@param ACopy The original object to copy from
					@return An Object specialized on AAllocatable containing the copies object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject> Object<AObject> CopyObject(const AObject& ACopy);

				/*!
					@brief Deletes an object allocated with the Allocator
					@param AAllocatable A pointer to the allocated object
					@par Thread Safety
						This function requires no thread safety
				*/
				template <class AObject> void DeleteObject(AObject* const AAllocatable) noexcept;
			};
		};
	};
};