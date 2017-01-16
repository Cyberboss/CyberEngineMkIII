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
			protected:
				/*!
					@brief Construct an Allocator
					@param AHeap The Heap the allocator will use
					@par Thread Safety
						Function calls should be synchronized
				*/
				Allocator(Heap& AHeap) noexcept;				

				template <class AObject, typename AConstructor, typename... AArgs> AObject* AllocateObject(const std::true_type AIgnored, AArgs&&... AArguments);
				template <class AObject, typename AConstructor, typename... AArgs> AObject* AllocateObject(const std::false_type AIgnored, AArgs&&... AArguments);
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
					@brief Allocates the Object specified by AObject using a specified Constructor
					@tparam AObject The type of object to allocate
					@tparam AConstructor The constructor for AObject to use
					@tparam AArgs The arguments types of AObject's constructor
					@param AArguments The arguments of AObject's constructor
					@return An Object specialized on AObject containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject, class AConstructor, typename... AArgs> Object<AObject> ConstructObject(AArgs&&... AArguments);
				/*!
					@brief Allocates the Object specified by AObject using the default constructor
					@tparam AObject The type of object to allocate
					@tparam AArgs The arguments types of AObject's constructor
					@param AArguments The arguments of AObject's constructor
					@return An Object specialized on AObject containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject, typename... AArgs> Object<AObject> NewObject(AArgs&&... AArguments);
				/*!
					@brief Allocates the Object specified by AObject using the empty constructor
					@tparam AObject The type of object to allocate
					@return An Object specialized on AObject containing the allocated object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject> Object<AObject> NewObject(void);

				/*!
					@brief Copys an Object of type AObject
					@tparam AObject The type of object to copy
					@param ACopy The original object to copy from
					@return An Object specialized on AObject containing the copies object
					@par Thread Safety
						This function requires no thread safety
					@attention Throws dependant on called constructor
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the heap does not have the space for the allocation and more system memory cannot be requested
				*/
				template <class AObject> Object<AObject> CopyObject(const AObject& ACopy);

				/*!
					@brief Deletes an object allocated with the Allocator
					@param AAllocated A pointer to the allocated object
					@par Thread Safety
						This function requires no thread safety
				*/
				template <class AObject> void DeleteObject(AObject* const AAllocated) noexcept;
			};
		};
	};
};