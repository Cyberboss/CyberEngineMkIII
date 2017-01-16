//! @file Containers.hpp Aliasings of STL containers to use the correct allocator types
#pragma once

namespace CYB {
	namespace API {
		namespace Interop { template <class AType> class STLAllocator; };
		//! @brief Contains usings of STL contianer types
		namespace Container {
			//! @brief std::deque algorithms using the CyberEngine's allocator
			template <typename AType> using Deque = std::deque<AType, Interop::STLAllocator<AType>>;
			//! @brief std::vector algorithms using the CyberEngine's allocator
			template <typename AType> using Vector = std::vector<AType, Interop::STLAllocator<AType>>;
			//! @brief std::stack algorithms using the CyberEngine's allocator
			template <typename AType, typename AContainer = Deque<AType>> using Stack = std::stack<AType, AContainer>;
		};
		//! @brief UniquePointer that uses our Allocator
		template <typename AType> class UniquePointer : public std::unique_ptr<AType, void(*)(AType* const)> {
		private:
			/*!
				@brief Deletes @p APointer using the Allocator
				@param APointer The pointer to delete
				@par Thread Safety
					This function requires no thread safety
			*/
			static void Delete(AType* const APointer) noexcept;
		public:
			using unique_ptr<AType>::unique_ptr;

			/*!
				@brief Initialize an empty UniquePointer
				@par Thread Safety
					This function requires no thread safety
			*/
			constexpr UniquePointer() noexcept;
			/*!
				@brief Initialize an empty UniquePointer
				@param AIgnored This parameter is included for STL compliance
				@par Thread Safety
					This function requires no thread safety
			*/
			constexpr UniquePointer(const nullptr_t AIgnored) noexcept;
			/*!
				@brief Initialize a UniquePointer with @p APointer
				@param APointer The pointer the UniquePointer will contain
				@par Thread Safety
					This function requires no thread safety
			*/
			explicit UniquePointer(pointer const APointer) noexcept;
		};
	};
};

#include "STLPrototypes.inl"