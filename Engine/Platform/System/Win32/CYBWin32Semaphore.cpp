//! @file CYBWin32Semaphore.cpp Implements CYB::Platform::System::Semaphore for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Win32;

CYB::Platform::System::Semaphore::Semaphore() {
	Core().FModuleManager.Call<Modules::Kernel32::InitializeCriticalSection>(&FCritSection);
	Core().FModuleManager.Call<Modules::Kernel32::InitializeConditionVariable>(&FCondVar);
}

CYB::Platform::System::Semaphore::~Semaphore(){
	//See https://stackoverflow.com/questions/28975958/why-does-windows-have-no-deleteconditionvariable-function-to-go-together-with
	Core().FModuleManager.Call<Modules::Kernel32::DeleteCriticalSection>(&FCritSection);
}