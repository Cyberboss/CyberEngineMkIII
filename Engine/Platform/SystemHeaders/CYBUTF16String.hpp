//! @file CYBUTF16String.hpp Defines the Win32 only UTF16 converter

namespace CYB {
	namespace API {
		namespace String {
			//! @brief UTF-16 String enabled only under windows
			class UTF16 : public Dynamic {
			private:
				/*!
					@brief Construct a raw allocated UTF16 wide char array casted to a char array given a UTF8 string
					@param AUTF8 The UTF8 string to construct from
					@param[out] ALengthReference Contains the length of the allocated buffer - 1 upon successful exit
					@return A allocated char pointer to a wide string
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static char* SetupData(const UTF8& AUTF8, unsigned long long& ALengthReference);

				/*!
					@brief Construct a new UTF16 from a UTF8 string
					@param AUTF8 The UTF8 string to construct from
					@param ALengthReference Ignored and used internally
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				UTF16(const UTF8& AUTF8, unsigned long long ALengthReference);
			public:
				/*!
					@brief Create a new UTF8 string given a wide char array
					@param AWString The wide char array to construct from
					@return A UTF8 string representing the UTF16 string
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static UTF8 ToUTF8(const wchar_t* AWString);

				/*! 
					@brief Empty string constructor
					@par Thread Safety
						This function requires no thread safety
				*/
				UTF16() noexcept = default;
				/*!
					@brief Construct a new UTF16 a UTF8 string
					@param AUTF8 The UTF8 string to construct from
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				UTF16(const UTF8& AUTF8);

				/*!
					@brief Get the underlying wide char array
					@return The underlying wide char array
					@par Thread Safety
						This function requires sychronization at the object level
				*/
				wchar_t* WideData(void) noexcept;
				/*!
					@brief Get the underlying const wide char array
					@return The underlying const wide char array
					@par Thread Safety
						This function requires sychronization at the object level
				*/
				const wchar_t* WString(void) const noexcept;
			};
		};
	};
};
