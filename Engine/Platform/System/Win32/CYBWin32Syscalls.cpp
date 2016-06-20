#include "CYB.hpp"

//kernel changes every day and isn't documented, we have to use the winapi

unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber) noexcept {
	API::Assert::Equal(ACallNumber, GET_CURRENT_PROCESS);
	return reinterpret_cast<unsigned long long>(Win32::GetCurrentProcess());
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber, const Union64 AArg1) noexcept {
	switch (ACallNumber) {
	case TERMINATE_PROC:
		Win32::TerminateProcess(AArg1.FPointer, 0);
		return 0;
	case CLOSE_LIBRARY:
		Win32::FreeLibrary(static_cast<Win32::HMODULE>(AArg1.FPointer));
		return 0;
	case LOAD_LIBRARY:
	{
		CYB::API::String::Static String(static_cast<const char* const>(AArg1.FPointer));
		wchar_t Unicode[MAX_PATH];
		//+1 for the \0
		if (Win32::MultiByteToWideChar(CP_UTF8, 0, String.CString(), static_cast<int>(String.RawLength() + 1), Unicode, MAX_PATH) > MAX_PATH)
			return 0;
		return reinterpret_cast<unsigned long long>(Win32::LoadLibraryW(Unicode));
	}
	case LOAD_SYMBOL:
	case GET_CURRENT_PROCESS:
	default:
		API::Assert::HCF();
	}
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept {
	using namespace Win32;
	switch (ACallNumber) {
	case LOAD_SYMBOL:
		return reinterpret_cast<unsigned long long>(GetProcAddress(static_cast<HMODULE>(AArg1.FPointer), static_cast<const char* const>(AArg2.FPointer)));
	case TERMINATE_PROC:
	case CLOSE_LIBRARY:
	case LOAD_LIBRARY:
	case GET_CURRENT_PROCESS:
	default:
		API::Assert::HCF();
	}
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	API::Assert::HCF();
}
void CYB::Platform::System::Sys::VerifyArgumentCount(const CallNumber, const unsigned long long) noexcept {
	API::Assert::HCF();
}