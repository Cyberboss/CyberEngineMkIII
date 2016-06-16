//! @file StaticString.hpp Contains the definitions of a basic string
#pragma once
namespace CYB {
	namespace API {
		namespace String {
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
				Static(const Static&) noexcept = default;
				Static(Static&& AMove) noexcept = default;	//!< @brief See @ref structors
				Static& operator=(Static&& AMove) noexcept = default;	//!< @brief See @ref structors

				//! @copydoc CYB::API::String::CStyle::RawLength()
				unsigned int Length(void) const noexcept override;
			};
		};
	};
};
#include "StaticString.inl"