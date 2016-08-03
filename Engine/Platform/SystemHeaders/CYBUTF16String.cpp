//! @file CYBUTF16String.cpp Implements the Win32 only UTF16 converter
#include "CYB.hpp"

char* CYB::API::String::UTF16::SetupData(const UTF8& AUTF8, unsigned long long& ALengthReference) {
	const auto BufferSize(Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, nullptr, 0));
	ALengthReference = BufferSize * sizeof(wchar_t);
	Assert::LessThan<unsigned long long>(ALengthReference, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	auto NewData(static_cast<wchar_t*>(Allocator().FHeap.Alloc(static_cast<int>(ALengthReference))));
	if (CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::MultiByteToWideChar>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AUTF8.CString(), -1, NewData, BufferSize) == 0) {
		Allocator().FHeap.Free(NewData);
		throw Exception::SystemData(Exception::SystemData::STRING_VALIDATION_FAILURE);
	}
	return reinterpret_cast<char*>(NewData);
}
CYB::API::String::UTF8 CYB::API::String::UTF16::ToUTF8(const wchar_t* AWString) {
	const auto BufferSize(CYB::Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, nullptr, 0, nullptr, nullptr));
	auto NewData(static_cast<char*>(Allocator().FHeap.Alloc(BufferSize)));
	if (Core().FModuleManager.FK32.Call<CYB::Platform::Modules::Kernel32::WideCharToMultiByte>(CYB::Platform::Win32::UINT(CP_UTF8), CYB::Platform::Win32::DWORD(0), AWString, -1, NewData, BufferSize, nullptr, nullptr) == 0)
		throw Exception::SystemData(Exception::SystemData::STRING_VALIDATION_FAILURE);
	return UTF8(FromData(NewData));
}

CYB::API::String::UTF16::UTF16(const UTF8& AUTF8) :
	UTF16(AUTF8, 0)
{}

CYB::API::String::UTF16::UTF16(const UTF8& AUTF8, unsigned long long ALengthReference) :
	Dynamic(SetupData(AUTF8, ALengthReference))
{
	API::Assert::LessThan(ALengthReference, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	FLength = static_cast<int>(ALengthReference);
}

wchar_t* CYB::API::String::UTF16::WideData(void) noexcept {
	return reinterpret_cast<wchar_t*>(FData);
}
const wchar_t* CYB::API::String::UTF16::WString(void) const noexcept {
	return reinterpret_cast<const wchar_t*>(FData);
}