#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(void) noexcept {
	//Self terminating is actually safer than exiting due to locks and shit, also allows for us to kill ourselves and other processes in one line
	//No FK32 because this can be called without Core
	Win32::TerminateProcess(FHandle, 0);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	//No FK32 because this can be called without Core
	return Process(Win32::GetCurrentProcess());
}

CYB::Platform::System::Implementation::Process::Process(Win32::HANDLE AHandle) noexcept :
	FHandle(AHandle)
{}

CYB::Platform::System::Implementation::Process::Process(Process&& AMove) noexcept :
	FHandle(AMove.FHandle)
{
	AMove.FHandle = nullptr;
}

CYB::Platform::System::Implementation::Process& CYB::Platform::System::Implementation::Process::operator=(Process&& AMove) noexcept {
	FHandle = AMove.FHandle;
	AMove.FHandle = nullptr;
	return *this;
}

CYB::Platform::System::Implementation::Process::~Process() {
	if(FHandle != nullptr)
		Core().FModuleManager.FK32.Call<Modules::Kernel32::CloseHandle>(FHandle);
}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	return Core().FModuleManager.FK32.Call<Modules::Kernel32::WaitForSingleObject>(FHandle, Win32::DWORD(0)) == WAIT_TIMEOUT;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	if (Active()) {
		const auto Result(Core().FModuleManager.FK32.Call<Modules::Kernel32::GetProcessId>(FHandle));
		if (Result != 0)
			return Result == Core().FModuleManager.FK32.Call<Modules::Kernel32::GetProcessId>(ARHS.FHandle);
	}
	return false;
}