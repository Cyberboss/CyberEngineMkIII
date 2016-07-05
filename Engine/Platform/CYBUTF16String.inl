//! @file CYBUTF16.inl Implements the Win32 only UTF16 converter

namespace CYB {
	namespace API {
		namespace String {
			//! @brief UTF-16 String enabled only under windows
			class UTF16 : public Dynamic {
			private:
				/*!
					@brief Construct a raw allocated UTF16 wide char array casted to a char array given a UTF8 string
					@param AUTF8 The UTF8 string to construct from
					@return A allocated char pointer to a wide string
					@par Thread Safety
						This functions requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static inline char* SetupData(const UTF8& AUTF8) {
					const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, nullptr, 0));
					const auto TotalSize(BufferSize * sizeof(wchar_t));
					Assert::LessThan<unsigned long long>(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
					auto NewData(static_cast<wchar_t*>(Allocator().FHeap.Alloc(static_cast<int>(TotalSize))));
					if (CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, NewData, BufferSize) == 0) {
						Allocator().FHeap.Free(NewData);
						throw Exception::Internal(Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					}
					return reinterpret_cast<char*>(NewData);
				}
			public:
				/*!
					@brief Create a new UTF8 string given a wide char array
					@param AWString The wide char array to construct from
					@return 
					@par Thread Safety
						This functions requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				inline static UTF8 ToUTF8(const wchar_t* AWString) {
					const auto BufferSize(CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, nullptr, 0, nullptr, nullptr));
					auto NewData(static_cast<char*>(Allocator().FHeap.Alloc(BufferSize)));
					if (Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, NewData, BufferSize, nullptr, nullptr) == 0)
						throw Exception::Internal(Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					return UTF8(FromData(NewData));
				}

				/*!
					@brief Construct a new UTF16 a UTF8 string
					@param AUTF8 The UTF8 string to construct from
					@par Thread Safety
						This functions requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				inline UTF16(const UTF8& AUTF8) :
					Dynamic(SetupData(AUTF8))
				{
					++FLength;	//null terminator is 2 bytes
				}

				/*!
					@brief Get the underlying wide char array
					@return The underlying wide char array
					@par Thread Safety
						This functions requires sychronization at the object level
				*/
				inline wchar_t* WideData(void) noexcept {
					return reinterpret_cast<wchar_t*>(FData);
				}
				/*!
					@brief Get the underlying const wide char array
					@return The underlying const wide char array
					@par Thread Safety
						This functions requires sychronization at the object level
				*/
				inline const wchar_t* WString(void) const noexcept {
					return reinterpret_cast<const wchar_t*>(FData);
				}
			};
		};
	};
};
