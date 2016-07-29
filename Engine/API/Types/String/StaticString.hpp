//! @file StaticString.hpp Contains the definitions of a basic string
#pragma once
namespace CYB {
	namespace API {
		namespace String {
			//! @brief A string pointing to unchanging data in the stack above it or the data segment. Must have UTF-8 encoding
			class Static : public CStyle {
			public:
				/*!
					@brief Construct an empty static string
					@par Thread Safety
						This function requires no thread safety
				*/
				Static() noexcept;
				/*!
					@brief Construct a static string
					@par Thread Safety
						This function requires no thread safety
				*/
				Static(const char* const AData) noexcept;
				Static(const Static&) noexcept = default;	//!< @brief See @ref structors
				Static(Static&& AMove) noexcept = default;	//!< @brief See @ref structors
				Static& operator=(Static&& AMove) noexcept = default;	//!< @brief See @ref structors

				//! @copydoc CYB::API::String::CStyle::Length()
				int Length(void) const noexcept override;
			};
		};
	};
};
#include "StaticString.inl"