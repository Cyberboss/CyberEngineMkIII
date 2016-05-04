//! @file String.hpp Contains the definitions of a basic string
#pragma once
namespace CYB {
	namespace API {
		namespace String {
			class Static : public CStyle {
			public:
				/*!
					@brief Construct a static string
					@param AData The data to populate the string with. Must remaing valid for the lifetime of the object
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				Static(char* const AData);

				/*!
					@brief Equivalent to CYB::API::String::CStyle::RawLength
				*/
				unsigned long long Length(void) const override;
			};
		};
	};
};
#include "StaticString.inl"