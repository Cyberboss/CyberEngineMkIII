//! @file STLAllocator.hpp Defines CYB::API::STLAllocator
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Remaps STL style allocations to the CyberEngine Allocator. Based on std::allocator, usable on STL containers
			template <class AType> class STLAllocator {
			public:
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::rebind</a>
				template<class AOther> struct rebind { typedef STLllocator<AOther> other; };

				using value_type = AType;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
				using propagate_on_container_move_assignment = std::false_type;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
				using is_always_equal = std::true_type;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
			public:
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/allocate">std::allocator::allocate</a>
				static AType* allocate(const std::size_t ACount);
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/deallocate">std::allocator::deallocate</a>
				static void deallocate(AType* const AObject, const std::size_t ACount) noexcept;

				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/allocator">std::allocator::allocator</a>
				STLAllocator() noexcept = default;
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/allocator">std::allocator::allocator</a>
				STLAllocator(const STLAllocator& ACopy) noexcept = default;
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/allocator">std::allocator::allocator</a>
				template <class AOtherType> STLAllocator(const AOtherType& AOther) noexcept;

				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/operator_cmp">std::allocator operator_cmp</a>
				template<class ARType> constexpr bool operator==(const STLAllocator<ARType>& ARHS) const noexcept;
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/operator_cmp">std::allocator operator_cmp</a>
				template<class ARType> constexpr bool operator!=(const STLAllocator<ARType>& ARHS) const noexcept;
			};
		};
	};
};