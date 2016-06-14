#include "CYB.hpp"

CYB::Platform::System::Mutex::Mutex() {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::InitializeCriticalSection>(&FCriticalSection);
}

CYB::Platform::System::Mutex::~Mutex() {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::DeleteCriticalSection>(&FCriticalSection);
}

void CYB::Platform::System::Mutex::Lock(void) const noexcept {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::EnterCriticalSection>(&FCriticalSection);
}

bool CYB::Platform::System::Mutex::TryLock(void) const noexcept {
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::TryEnterCriticalSection>(&FCriticalSection) == TRUE;
}

void CYB::Platform::System::Mutex::Unlock(void) const noexcept {
	Core().FModuleManager.FK32.Call<Modules::Kernel32::LeaveCriticalSection>(&FCriticalSection);
}