//! @file CYBWin32Path.cpp Implements CYB::Platform::System::Path for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

namespace CYB {
	namespace API {
		namespace String {
			//! @brief UTF-16 String enabled only under windows
			class UTF16 : public Dynamic {
			private:
				wchar_t*& WideData(void) noexcept {
					return reinterpret_cast<wchar_t*&>(FData);
				}
				char* SetupData(const UTF8& AUTF8) {
					const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(UINT(CP_UTF8), DWORD(0), AUTF8.CString(), -1, nullptr, 0));
					const auto TotalSize(BufferSize * sizeof(wchar_t));
					Assert::LessThan<unsigned long long>(TotalSize, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
					auto NewData(static_cast<wchar_t*>(Allocator().FHeap.Alloc(static_cast<int>(TotalSize))));
					Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(UINT(CP_UTF8), DWORD(0), AUTF8.CString(), -1, NewData, BufferSize);
					return reinterpret_cast<char*>(NewData);
				}
			public:
				UTF16(const UTF8& AUTF8) :
					Dynamic(SetupData(AUTF8))
				{
					++FLength;
				}
				const wchar_t* WString(void) const noexcept {
					return reinterpret_cast<const wchar_t*>(FData);
				}
				UTF8 ToUTF8(void) const {
					const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(UINT(CP_UTF8), DWORD(0), WString(), -1, nullptr, 0, nullptr, nullptr));
					auto NewData(static_cast<char*>(Allocator().FHeap.Alloc(BufferSize)));
					if (Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(UINT(CP_UTF8), DWORD(0), WString(), -1, NewData, BufferSize, nullptr, nullptr) == 0)
						throw CYB::Exception::Internal(CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					return UTF8(NewData);
				}
			};
		};
	};
};