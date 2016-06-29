//! @file CYBUTF16.inl Implements the Win32 only UTF16 converter

namespace CYB {
	namespace API {
		namespace String {
			//! @brief UTF-16 String enabled only under windows
			class UTF16 : public Dynamic {
			private:
				inline char* SetupData(const UTF8& AUTF8) {
					const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, nullptr, 0));
					const auto TotalSize(BufferSize * sizeof(wchar_t));
					Assert::LessThan<unsigned long long>(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
					auto NewData(static_cast<wchar_t*>(Allocator().FHeap.Alloc(static_cast<int>(TotalSize))));
					CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, NewData, BufferSize);
					return reinterpret_cast<char*>(NewData);
				}
			public:
				inline UTF16(const UTF8& AUTF8) :
					Dynamic(SetupData(AUTF8))
				{
					++FLength;
				}
				inline wchar_t*& WideData(void) noexcept {
					return reinterpret_cast<wchar_t*&>(FData);
				}
				inline const wchar_t* WString(void) const noexcept {
					return reinterpret_cast<const wchar_t*>(FData);
				}
				inline static UTF8 ToUTF8(const wchar_t* AWString) {
					const auto BufferSize(CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, nullptr, 0, nullptr, nullptr));
					auto NewData(static_cast<char*>(Allocator().FHeap.Alloc(BufferSize)));
					if (Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, NewData, BufferSize, nullptr, nullptr) == 0)
						throw Exception::Internal(Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					return UTF8(NewData);
				}
			};
		};
	};
};
