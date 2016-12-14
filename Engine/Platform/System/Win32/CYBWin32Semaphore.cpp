//! @file CYBWin32Semaphore.cpp Implements CYB::Platform::System::Semaphore for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::Semaphore::Semaphore() :
	FServiceCount(1),
	FWakeCount(0),
	FSleepCount(0) 
{
	Core().FModuleManager.Call<Modules::Kernel32::InitializeCriticalSection>(&FCritSection);
	Core().FModuleManager.Call<Modules::Kernel32::InitializeConditionVariable>(&FCondVar);
}

CYB::Platform::System::Semaphore::~Semaphore(){
	//See https://stackoverflow.com/questions/28975958/why-does-windows-have-no-deleteconditionvariable-function-to-go-together-with
	Core().FModuleManager.Call<Modules::Kernel32::DeleteCriticalSection>(&FCritSection);
}

void CYB::Platform::System::Semaphore::Lock(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::EnterCriticalSection>(&FCritSection);
}

void CYB::Platform::System::Semaphore::Unlock(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::LeaveCriticalSection>(&FCritSection);
}

void CYB::Platform::System::Semaphore::EnterCV(void) noexcept {
	API::Assert::LessThan(0, Core().FModuleManager.Call<Modules::Kernel32::SleepConditionVariableCS>(&FCondVar, &FCritSection, INFINITE));
}

void CYB::Platform::System::Implementation::Semaphore::WakeAll(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::WakeAllConditionVariable>(&FCondVar);
}

void CYB::Platform::System::Implementation::Semaphore::WakeOne(void) noexcept {
	Core().FModuleManager.Call<Modules::Kernel32::WakeConditionVariable>(&FCondVar);
}