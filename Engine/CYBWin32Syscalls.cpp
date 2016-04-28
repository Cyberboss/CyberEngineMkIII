//! @file CYBWin32Syscalls.cpp Dummy implementation of Windows Syscalls
#include "CYB.hpp"

//kernel changes every day and isn't documented, we have to use the winapi

unsigned long long CYB::Platform::Sys::DoCall(const CallNumber) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64, const Union64) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64) {
	CYB::API::HCF();
}
unsigned long long CYB::Platform::Sys::DoCall(const CallNumber, const Union64, const Union64, const Union64, const Union64, const Union64, const Union64) {
	CYB::API::HCF();
}
void CYB::Platform::Sys::VerifyArgumentCount(const CallNumber, const size_t) {
	CYB::API::HCF();
}