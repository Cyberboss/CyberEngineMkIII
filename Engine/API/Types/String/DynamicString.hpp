//! @file UTF8String.hpp Defines CYB::API::String::Dynamic
#pragma once
namespace CYB {
	namespace API {
		namespace String {
			//! @brief Used for string of allocated variable length
			class Dynamic : public CStyle {
			private:
				/*!
					@brief Allocates data for and initializes a copy of a CStyle string
					@param AData The data to copy
					@param ALength The length of AData to copy. Looks instead for '\0' if set to -1
					@return An allocated char array that contains the contents of AData
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current hep runs out of memory
				*/
				static char* CopyCStyle(const CStyle& AData, int ALength);

				/*!
					@brief Allocates data for a string that represents AValue
					@param AValue The value to make a string
					@return A string representation of @p AValue
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current hep runs out of memory
				*/
				static Dynamic MakeNumberString(const unsigned long long AValue);

				/*!
					@brief Frees the data of the current string
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				void DeallocateData(void);
			protected:
				/*!
					@brief Construct a Dynamic string from a pre-allocated char array
					@param AData The pre-allocated char array, will be deallocated along with the Dynamic
					@return The new Dynamic
					@par Thread Safety
						This function requires no thread safety
				*/
				static Dynamic FromData(char* const AData) noexcept;

				/*!
					@brief Construct a Dynamic string from a pre-allocated char array
					@param AData The pre-allocated char array, will be deallocated along with the Dynamic
					@par Thread Safety
						This function requires no thread safety
				*/
				Dynamic(char* const AData) noexcept;
			public:
				/*!
					@brief Allocate a char array that is @p ASize bytes in length. Call @p APopulateData with the char array as a parameter and then calculate the length
					@param ASize The amount of bytes to preallocate
					@param APopulateData The data population function, takes a void* as an argument and has a length of @p ASize and returns a bool
					@tparam ALambda The type of @p APopulateData
					@param ADynamic The Dynamic to instantiate
					@return The return value of @p APopulateData
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory. ADynamic will not be modified
				*/
				template <typename ALambda> static bool BuildAndPopulateBuffer(const int ASize, const ALambda APopulateData, Dynamic& ADynamic);

				/*!
					@brief Construct an empty Dynamic string
					@par Thread Safety
						This function requires no thread safety
				*/
				Dynamic() noexcept;
				/*!
					@brief Contstruct a Dynamic from a number
					@param AValue The value to translate to a string
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic(const unsigned long long AValue);
				/*!
					@brief Contstruct a Dynamic from a number
					@param AValue The value to translate to a string
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic(const long long AValue);
				/*!
					@brief Construct a Dynamic string from a char array
					@param AData The char array
					@param ALength The length of AData to copy. Looks instead for '\0' if set to -1
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic(const char* const AData, const int ALength = -1);
				/*!
					@brief Construct a Dynamic string. This will allocate enough data to copy the contents of @p AData
					@param AData The data to populate the char array with
					@param ALength The length of AData to copy. Uses @p AData's RawLength() if set to -1
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic(const CStyle& AData, const int ALength = -1);
				/*!
					@brief Copies the FData of @p ACopy for its FLength bytes
					@param ACopy a reference to the existing Dynamic to copy
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic(const Dynamic& ACopy);
				Dynamic(Dynamic&& AMove) noexcept;	//!< @brief See @ref structors
				Dynamic& operator=(Dynamic&& AMove) noexcept;	//!< @brief See @ref structors
				~Dynamic();	//!< @brief Frees allocated string

				/*!
					@brief Appends to a Dynamic string and returns a new one
					@param ARHS The string to append
					@return The new string
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic operator+(const CStyle& ARHS) const;
				/*!
					@brief Appends to a Dynamic string in place
					@param ARHS The string to append
					@return A self reference
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic& operator+=(const CStyle& ARHS);
				/*!
					@brief Get a substring of the contained string
					@param AIndex the starting index. Must be less than Length
					@param ALength The number of bytes to use. This plus @p AStart must be less than or equal to Length
					@return The substring @p AIndex, @p ALength of the original string
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Dynamic SubString(const int AIndex, const int ALength) const;
				/*!
					@brief Split the string into a Deque of other strings based on a seperator
					@param ASeparator The seperator for the strings
					@return A Deque of the split strings
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Container::Deque<Dynamic> Tokenize(const char ASeparator) const;

				/*!
					@brief Move the null terminator of the string so that the readable length is @p AMaxBytes
					@param AMaxBytes The maximum index of the new null terminator
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				virtual void Shrink(const int AMaxBytes) noexcept;

				//! @copydoc CYB::API::String::CStyle::Length()
				int Length(void) const noexcept override;
			};
		};
	};
};