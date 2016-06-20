#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void CYB::Platform::System::Process::Terminate(void) noexcept {
	if (FPID == static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS)))
		Sys::Call(Sys::EXIT_PROC);
	else
		Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	return Process(static_cast<pid_t>(Sys::Call(Sys::GET_CURRENT_PROCESS)));
}

CYB::Platform::System::Implementation::Process::Process(const pid_t APID) noexcept :
	FPID(APID)
{}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	return Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, 0) == 0 
		|| errno == EPERM;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	return (FPID == ARHS.FPID) && Active();
}