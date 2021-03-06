//! @file CYBInterop.hpp Defines the implementations of virtual functions defined in CYB::API::Interop
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief Implements InteropAllocation for the public allocator
		class Allocator : public API::Interop::Allocator {
		private:
			/*!
				@brief Template allocator for Allocatables
				@tparam AAllocatable The Allocatable that inherits from an API Allocatable
				@param AConstructor The Constructor for AAllocatable
				@return A pointer to a context heap allocated AAllocatable
				@par Thread Safety
					Dependant on called constructors
				@attention Throws dependant on called constructor
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
			*/
			template <class AAllocatable> AAllocatable* DoAllocation(API::Interop::Constructor<void>& AConstructor);
			//! @copydoc CYB::API::Interop::Allocator::InteropAllocation
			void* InteropAllocation(const API::Interop::Allocatable::ID AID, API::Interop::Constructor<void>& AConstructor) final override;
		public:
			//! @copydoc CYB::API::Interop::Allocator::Allocator(CYB::API::Heap&)
			Allocator(API::Heap& AHeap) noexcept;

			/*!
				@brief Allocates the Object specified by AAllocatable and returns a raw pointer which must be deleted using the Delete method of this object
				@tparam AObject The type of object to allocate
				@tparam AArgs The arguments types of AObject's constructor
				@param AArguments The arguments of AObject's constructor
				@return A pointer to an AObject allocated on this allocator's Heap
				@par Thread Safety
					This function requires no thread safety
				@attention Throws dependant on called constructor
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the Heap does not have the space for the allocation and more system memory cannot be requested
			*/
			template <class AObject, typename... AArgs> AObject* RawObject(AArgs&&... AArguments);
		};

		//! @brief Implements the engine level functions for API::Interop::Contexts
		class Context : public API::Interop::Context {
		private:
			Allocator FAllocatorObject;	//!< @brief The Allocator for this Context
		public:
			/*!
				@brief Setup the Context, optionally making it the current one
				@param AHeap The Heap for the Allocator to use
				@param ALogger The Logger to use
				@param AMakeCurrent If true, MakeCurrent will be called during the constructor
				@par Thread Safety
					This function requires no thread safety
			*/
			Context(API::Heap& AHeap, API::Logger& ALogger, const bool AMakeCurrent) noexcept;

			/*!
				@brief Set the current thread's Context singleton to this Context
				@par Thread Safety
					This function requires no thread safety
			*/
			void MakeCurrent(void) noexcept;
		};

		//! @brief Context switching RAII helper
		class PushContext {
		private:
			Context& FOldContext;	//!< @brief The context to pop upon destruction
		public:
			/*!
				@brief Saves the current Context and calls MakeCurrent on @p ANewContext
				@param ANewContext The new Context to push
				@par Thread Safety
					This function requires no thread safety
			*/
			PushContext(Context& ANewContext) noexcept;
			//! @brief Calls MakeCurrent on FOldContext
			~PushContext();
		};
	};
};

#include "CYBInterop.inl"