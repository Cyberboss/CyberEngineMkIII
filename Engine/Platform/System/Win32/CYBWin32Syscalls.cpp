#include "CYB.hpp"

//kernel changes every day and isn't documented, we have to use the winapi

unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64, const Union64) noexcept {
	CYB::API::Assert::HCF();
}
void CYB::Platform::System::Sys::VerifyArgumentCount(const CallNumber, const unsigned long long) noexcept {
	CYB::API::Assert::HCF();
}