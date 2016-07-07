#include "CYB.hpp"

using namespace CYB::Platform::Posix;
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber)
		: "rax");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1)
		: "rax", "rdi");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1), "irm" (AArg2)
		: "rax", "rdi", "rsi");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"movq %4, %%rdx\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1), "irm" (AArg2), "irm" (AArg3)
		: "rax", "rdi", "rsi", "rdx");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"movq %4, %%rdx\n\t"
		"movq %5, %%r10\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1), "irm" (AArg2), "irm" (AArg3), "irm" (AArg4)
		: "rax", "rdi", "rsi", "rdx", "r10");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"movq %4, %%rdx\n\t"
		"movq %5, %%r10\n\t"
		"movq %6, %%r8\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1), "irm" (AArg2), "irm" (AArg3), "irm" (AArg4), "irm" (AArg5)
		: "rax", "rdi", "rsi", "rdx", "r10", "r8");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5, const Union64 AArg6) noexcept {
	unsigned long long Result;
	asm volatile("movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"movq %4, %%rdx\n\t"
		"movq %5, %%r10\n\t"
		"movq %6, %%r8\n\t"
		"movq %7, %%r9\n\t"
		"syscall\n\t"
		"movq %%rax, %0"
		: "=rm" (Result)
		: "irm" (ACallNumber), "irm" (AArg1), "irm" (AArg2), "irm" (AArg3), "irm" (AArg4), "irm" (AArg5), "irm" (AArg6)
		: "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9");
	return Result;
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber) noexcept {
	switch (ACallNumber) {
	case EXIT_PROC:
		return Call(EXIT, 0);
	case GET_CURRENT_PROCESS:
		return static_cast<unsigned long long>(getpid());
	default:
		API::Assert::HCF();
	}
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber, const Union64 AArg1) noexcept {
	API::Assert::Equal(ACallNumber, CLOSE_LIBRARY);
	return static_cast<unsigned long long>(dlclose(AArg1.FPointer));
}
unsigned long long CYB::Platform::System::Sys::LinkedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2) noexcept {
	switch (ACallNumber) {
	case LOAD_LIBRARY:
		return reinterpret_cast<unsigned long long>(dlopen(static_cast<const char*>(AArg1.FPointer), AArg2.FNumber));
	case LOAD_SYMBOL:
		return reinterpret_cast<unsigned long long>(dlsym(AArg1.FPointer, static_cast<const char*>(AArg2.FPointer)));
	case STAT:
		return static_cast<unsigned long long>(stat(static_cast<const char*>(AArg1.FPointer), static_cast<StatStruct*>(AArg2.FPointer)));
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
void CYB::Platform::System::Sys::VerifyArgumentCount(const CallNumber ACallNumber, const unsigned long long ANumArgs) noexcept {
	switch (ACallNumber) {
	case EXIT:
		API::Assert::Equal<unsigned long long>(ANumArgs, 1);
		break;
	case EXIT_PROC:
	case CLOSE_LIBRARY:
	case LOAD_LIBRARY:
	case LOAD_SYMBOL:
	case GET_CURRENT_PROCESS:
	case STAT:
		API::Assert::HCF();
	}
}