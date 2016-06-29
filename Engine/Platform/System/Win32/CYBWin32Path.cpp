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
				static UTF8 ToUTF8(const wchar_t* AWString) {
					const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(UINT(CP_UTF8), DWORD(0), AWString, -1, nullptr, 0, nullptr, nullptr));
					auto NewData(static_cast<char*>(Allocator().FHeap.Alloc(BufferSize)));
					if (Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(UINT(CP_UTF8), DWORD(0), AWString, -1, NewData, BufferSize, nullptr, nullptr) == 0)
						throw CYB::Exception::Internal(CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING);
					return UTF8(NewData);
				}
			};
		};
	};
};

CYB::API::String::UTF8 CYB::Platform::System::Path::LocateDirectory(const SystemPath ADirectory) {
	auto& MM(Core().FModuleManager);
	switch (ADirectory) {
	case SystemPath::EXECUTABLE_IMAGE:
	case SystemPath::EXECUTABLE: {
		auto Module(MM.FK32.Call<Modules::Kernel32::GetModuleHandleW>(nullptr));
		if (Module != nullptr) {
			wchar_t Buffer[MAX_PATH];
			if (MM.FK32.Call<Modules::Kernel32::GetModuleFileNameW>(Module, Buffer, DWORD(MAX_PATH)) != 0)
				if (ADirectory == SystemPath::EXECUTABLE) {
					if (MM.FShellAPI.Call<Modules::ShellAPI::PathRemoveFileSpecW>(Buffer) != 0) {
						auto WithoutDelimiter(API::String::UTF16::ToUTF8(Buffer));
						WithoutDelimiter += CYB::API::String::UTF8(CYB::API::String::Static(u8"/"));
						return WithoutDelimiter;
					}
					throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
				else
					return CYB::API::String::UTF16::ToUTF8(Buffer);
		}
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
	}
	case SystemPath::RESOURCE:
	case SystemPath::TEMPORARY:
	case SystemPath::USER:
	case SystemPath::WORKING:
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
}