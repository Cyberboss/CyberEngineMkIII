//! @file CStyleString.hpp Contains the definitions of a basic c string
#pragma once
namespace CYB {
	namespace API {
		namespace String {
			//! @brief A basic char contained string
			class CStyle {
			protected:
				char* FData;	//!< @brief String data. May be const, do not modify from this class
				int FLength;	//!< @brief Number of bytes before null terminator in FData
			protected:
				/*!
					@brief Construct a CStyle string
					@param AData The data to populate the char array with. Must be at least a pointer to a null terminating character
					@par Thread Safety
						This function requires no thread safety
				*/
				CStyle(char* const AData) noexcept;
				virtual ~CStyle() = default;
				/*!
					@brief Calculate the byte length of the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				int CalculateByteLength(void) const noexcept;
			public:
				/*!
					@brief Get the contained const CString
					@return The const char array of the CStyle
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				const char* CString(void) const noexcept;
				/*!
					@brief Get the number of characters in the contained string
					@return The number of characters in the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				virtual int Length(void) const noexcept = 0;
				/*!
					@brief Get the byte length of the contained CString
					@return The byte length of the contained CString
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				int RawLength(void) const noexcept;

				/*!
					@brief String equivalence check
					@param ARHS The string to compare against
					@return true if two strings contain the same data, false otherwise
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				bool operator==(const CStyle& ARHS) const noexcept;
			};
		};
	};
};
#include "CStyleString.inl"