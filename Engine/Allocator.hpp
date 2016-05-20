//! @file Allocator.hpp Defines CYB::API::Allocator
#pragma once
namespace CYB {
	namespace API {
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
			Allocator(Heap& AHeap);
		public:
			/*!
				@brief Retrieve the Allocator singleton
				@return A reference to the Allocator singleton
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			static Allocator& GetAllocator(void);

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

			template <class AType> Object<AType> CopyObject(const Object<AType>& ACopy);
		};
	};
	//! @brief See CYB::API::Allocator::GetAllocator
	API::Allocator& Allocator(void);
};

#include "Allocator.inl"
#include "Object.inl"