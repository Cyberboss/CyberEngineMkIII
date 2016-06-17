#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(void) noexcept {
	if (FPID == Posix::getpid())
		Sys::Call(Sys::EXIT_PROC);
	else
		Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) noexcept {
	return Process(Posix::getpid());
}

CYB::Platform::System::Implementation::Process::Process(const Posix::pid_t APID) noexcept :
	FPID(APID)
{}

bool CYB::Platform::System::Process::Active(void) const noexcept {
	using namespace Posix;
	return Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, 0) == 0 
		|| errno == EPERM;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const noexcept {
	return (FPID == ARHS.FPID) && Active();
}