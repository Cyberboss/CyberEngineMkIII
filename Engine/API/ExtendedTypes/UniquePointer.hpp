//! @file UniquePointer.hpp Defines the CYB extension of std::unique_ptr
#pragma once

namespace CYB {
	namespace API {
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
			constexpr UniquePointer(const std::nullptr_t AIgnored) noexcept;
			/*!
				@brief Initialize a UniquePointer with @p APointer
				@param APointer The pointer the UniquePointer will contain
				@par Thread Safety
					This function requires no thread safety
			*/
			explicit UniquePointer(AType* const APointer) noexcept;
		};
	};
};

#include "UniquePointer.inl"