//! @file STLAllocator.hpp Defines CYB::API::STLAllocator
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Remaps STL allocations to the CYB Allocator. Based on std::allocator usable on STL containers
			template <class AType> class STLAllocator {
			public:
				typedef AType value_type;
				typedef std::false_type propagate_on_container_move_assignment;
				typedef std::true_type is_always_equal;
			public:
				static AType* allocate(const std::size_t ACount);
				static void deallocate(AType* const AObject, const std::size_t ACount) noexcept;

				STLAllocator() noexcept = default;
				STLAllocator(const STLAllocator& ACopy) noexcept = default;
				template <class AOtherType> STLAllocator(const AOtherType& AOther) noexcept;

				template<class ARType> constexpr bool operator==(const STLAllocator<ARType>& ARHS) const noexcept;
				template<class ARType> constexpr bool operator!=(const STLAllocator<ARType>& ARHS) const noexcept;
			};
		};
	};
};