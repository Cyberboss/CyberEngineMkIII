//! @file CYBContext.hpp Defines the implementations of virtual functions defined in CYB::API::Interop
#pragma once

namespace CYB {
	namespace Engine {
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
	};
};