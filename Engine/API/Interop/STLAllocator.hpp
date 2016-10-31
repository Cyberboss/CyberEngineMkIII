//! @file STLAllocator.hpp Defines CYB::API::STLAllocator
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Remaps STL style allocations to the CyberEngine Allocator. Based on std::allocator, usable on STL containers
			template <typename AType> class STLAllocator {
			public:
				using pointer = AType*;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::pointer</a>
				using const_pointer = const AType*;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::const_pointer</a>
				using reference = AType&;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::reference</a>
				using const_reference = const AType&;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::const_reference</a>
				using size_type = unsigned long long;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::size_type</a>
				using difference_type = std::ptrdiff_t;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::difference_type</a>
				
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::rebind</a>
				template<typename AOther> class rebind { 
				public:
					using other = STLAllocator<AOther>;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::rebind::other</a>
				};	

				using value_type = AType;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
				using propagate_on_container_move_assignment = std::false_type;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
				using is_always_equal = std::true_type;	//!< @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator</a>
			public:
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::address</a>
				static pointer address(reference AObject) noexcept;
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator">std::allocator::address</a>
				static const_pointer address(const_reference AObject) noexcept;

				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/allocate">std::allocator::allocate</a>
				static AType* allocate(const std::size_t ACount);
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/deallocate">std::allocator::deallocate</a>
				static void deallocate(AType* const AObject, const std::size_t ACount) noexcept;

				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/construct">std::allocator::construct</a>
				template <typename AOther, typename... AArgs> static void construct(AOther* const APointer, AArgs&&... AArguments);
				//! @brief See <a href="http://en.cppreference.com/w/cpp/memory/allocator/destroy">std::allocator::destroy</a>
				template <typename AOther> static void destroy(AOther* const APointer) noexcept;

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