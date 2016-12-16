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
		};

		//! @brief Implements the engine level functions for API::Interop::Contexts
		class Context : public API::Interop::Context {
		public:
			/*!
				@brief Setup the Context, optionally making it the current one
				@param AAllocator The allocator to use
				@param AMakeCurrent If true, MakeCurrent will be called during the constructor
				@par Thread Safety
					This function requires no thread safety
			*/
			Context(Allocator& AAllocator, const bool AMakeCurrent) noexcept;

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