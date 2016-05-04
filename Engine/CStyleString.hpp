//! @file CStyleString.hpp Contains the definitions of a basic c string
#pragma once
namespace CYB {
	namespace API {
		namespace String {
			//! @brief A basic char contained string
			class CStyle {
			protected:
				char* FData;
				unsigned long long FLength;
			protected:
				/*!
					@brief Construct a CStyle string
					@param AData The data to populate the char array with. Must be at least a pointer to a null terminating character
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				CStyle(char* const AData);
				virtual ~CStyle() = default;
				/*!
					@brief Calculate the byte length of the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				unsigned long long CalculateByteLength(void) const;
			public:
				/*!
					@brief Get the contained CString
					@return The char array of the CStyle
					@par Thread Safety
						This function requires synchronization at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				char* CString(void);
				/*!
					@brief Get the contained const CString
					@return The const char array of the CStyle
					@par Thread Safety
						This function requires synchronization at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				const char* CString(void) const;
				/*!
					@brief Get the number of characters in the contained string
					@return The number of characters in the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				virtual unsigned long long Length(void) const = 0;
				/*!
					@brief Get the byte length of the contained CString
					@return The byte length of the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				unsigned long long RawLength(void) const;
			};
		};
	};
};
#include "CStyleString.inl"