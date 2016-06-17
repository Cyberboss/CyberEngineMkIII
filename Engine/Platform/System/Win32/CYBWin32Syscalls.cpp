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
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber) noexcept {
	API::Assert::HCF();
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
		return reinterpret_cast<unsigned long long>(Win32::LoadLibraryA(static_cast<const char*>(AArg1.FPointer)));
	case LOAD_SYMBOL:
	default:
		API::Assert::HCF();
	}
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept {
	switch (ACallNumber) {
	case LOAD_SYMBOL:
		return reinterpret_cast<unsigned long long>(Win32::GetProcAddress(static_cast<Win32::HMODULE>(AArg1.FPointer), static_cast<const char* const>(AArg2.FPointer)));
	case TERMINATE_PROC:
	case CLOSE_LIBRARY:
	case LOAD_LIBRARY:
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