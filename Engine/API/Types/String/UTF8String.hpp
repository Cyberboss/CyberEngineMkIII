//! @file UTF8String.hpp Defines CYB::API::String::UTF8
#pragma once

namespace CYB {
	namespace API {
		namespace String {
			//! @brief A variable length UTF-8 string
			class UTF8 : public Dynamic {
				friend class UTF16;
			private:
				/*!
					@brief Construct from a preallocated Dynamic array
					@param AData The Dynamic to inherit
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the string does not validate
				*/
				UTF8(char* const AData) noexcept;
			public:
				/*!
					@brief Validates if a given string is UTF8.
					@par Thread Safety
						This function requires synchronization of AString at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the string does not validate
				*/
				static bool Validate(const CStyle& AString) noexcept;

				UTF8() noexcept = default;	//!< @brief See @ref structors
				/*!
					@brief Construct a UTF8 string from raw data
					@param AData The data to use for construction, will be copied
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the string does not validate
				*/
				UTF8(const CStyle& AData);
				UTF8(const UTF8& ACopy) = default;	//!< @brief See @ref structors
				UTF8(UTF8&& AMove) noexcept = default;	//!< @brief See @ref structors
				UTF8& operator=(UTF8&& AMove) RELEASE_NOEXCEPT = default;	//!< @brief See @ref structors
				
				/*!
					@brief Appends to a Dynamic string and returns a new one
					@param ARHS The string to append
					@return The new string
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE. Thrown if the current heap runs out of memory
				*/
				UTF8 operator+(const UTF8& ARHS) const;
				/*!
					@brief Appends to a Dynamic string in place
					@param ARHS The string to append
					@return A self reference
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE. Thrown if the current heap runs out of memory
				*/
				UTF8& operator+=(const UTF8& ARHS);

				//! @copydoc CYB::API::String::CStyle::Length()
				int Length(void) const noexcept final override;
			};
		};
	};
};