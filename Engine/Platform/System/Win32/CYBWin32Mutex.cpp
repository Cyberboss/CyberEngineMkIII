//! @file CYBWin32Mutex.cpp Implements CYB::Platform::System::Mutex for Win32
#include "CYB.hpp"

CYB::Platform::System::Mutex::Mutex() {
	Core().FModuleManager.Call<Modules::Kernel32::InitializeCriticalSection>(&FCriticalSection);
}

CYB::Platform::System::Mutex::~Mutex() {
	Core().FModuleManager.Call<Modules::Kernel32::DeleteCriticalSection>(&FCriticalSection);
}

void CYB::Platform::System::Mutex::Lock(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::EnterCriticalSection>(&FCriticalSection);
	std::atomic_thread_fence(std::memory_order_acquire);
}

bool CYB::Platform::System::Mutex::TryLock(void) noexcept {
	const auto Result(Core().FModuleManager.Call<Modules::Kernel32::TryEnterCriticalSection>(&FCriticalSection) == TRUE);
	std::atomic_thread_fence(std::memory_order_acquire);
	return Result;
}

void CYB::Platform::System::Mutex::Unlock(void) noexcept {
	std::atomic_thread_fence(std::memory_order_release);
	Core().FModuleManager.Call<Modules::Kernel32::LeaveCriticalSection>(&FCriticalSection);
}