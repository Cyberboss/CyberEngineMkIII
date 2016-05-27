#include "CYB.hpp"

CYB::Platform::Mutex::Mutex() {
	Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::InitializeCriticalSection>(&FCriticalSection);
}

CYB::Platform::Mutex::~Mutex() {
	Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::DeleteCriticalSection>(&FCriticalSection);
}

void CYB::Platform::Mutex::Lock(void) {
	Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::EnterCriticalSection>(&FCriticalSection);
}

bool CYB::Platform::Mutex::TryLock(void) {
	return Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::TryEnterCriticalSection>(&FCriticalSection) == TRUE;
}

void CYB::Platform::Mutex::Unlock(void) {
	Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::LeaveCriticalSection>(&FCriticalSection);
}