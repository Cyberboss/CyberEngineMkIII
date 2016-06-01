#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(void) {
	//Self terminating is actually safer than exiting due to locks and shit
	//No FK32 because this can be called without Core
	Win32::TerminateProcess(FHandle, 0);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) {
	//No FK32 because this can be called without Core
	return Process(Win32::GetCurrentProcess());
}

CYB::Platform::System::Implementation::Process::Process(CYB::Platform::Win32::HANDLE AHandle) :
	FHandle(AHandle)
{}

CYB::Platform::System::Implementation::Process::Process(Process&& AMove) :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = nullptr;
}

CYB::Platform::System::Implementation::Process& CYB::Platform::System::Implementation::Process::operator=(Process&& AMove) {
	FHandle = AMove.FHandle;
	AMove.FHandle = nullptr;
	return *this;
}

CYB::Platform::System::Implementation::Process::~Process() {
	if(FHandle != nullptr)
		Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FHandle);
}
