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
}

bool CYB::Platform::System::Mutex::TryLock(void) noexcept {
	return Core().FModuleManager.Call<Modules::Kernel32::TryEnterCriticalSection>(&FCriticalSection) == TRUE;
}

void CYB::Platform::System::Mutex::Unlock(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::LeaveCriticalSection>(&FCriticalSection);
}