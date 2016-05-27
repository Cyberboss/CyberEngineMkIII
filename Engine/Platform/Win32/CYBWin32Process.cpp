#include "CYB.hpp"

void CYB::Platform::Process::Terminate(void) {
	//Self terminating is actually safer than exiting due to locks and shit
	//No FK32 because this can be called without Core
	Implementation::Win32::TerminateProcess(FHandle, 0);
}

CYB::Platform::Process CYB::Platform::Process::GetSelf(void) {
	//No FK32 because this can be called without Core
	return Process(Implementation::Win32::GetCurrentProcess());
}

CYB::Platform::Implementation::Process::Process(CYB::Platform::Implementation::Win32::HANDLE AHandle) :
	FHandle(AHandle)
{}

CYB::Platform::Implementation::Process::Process(Process&& AMove) :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = nullptr;
}

CYB::Platform::Implementation::Process& CYB::Platform::Implementation::Process::operator=(Process&& AMove) {
	FHandle = AMove.FHandle;
	AMove.FHandle = nullptr;
	return *this;
}

CYB::Platform::Implementation::Process::~Process() {
	if(FHandle != nullptr)
		Core().FModuleManager.FK32.Call<ModuleDefinitions::Kernel32::CloseHandle>(FHandle);
}
