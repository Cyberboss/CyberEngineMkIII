#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(void) {
	if (FPID == Posix::getpid())
		Sys::Call(Sys::EXIT, 0);
	Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL);
}

CYB::Platform::System::Process CYB::Platform::System::Process::GetSelf(void) {
	return Process(Posix::getpid());
}

CYB::Platform::System::Implementation::Process::Process(CYB::Platform::Posix::pid_t APID) :
	FPID(APID)
{}

bool CYB::Platform::System::Process::Active(void) const {
	using namespace Posix;
	return Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, 0) == -1 
		&& errno == ESRCH;
}

bool CYB::Platform::System::Process::operator==(const Process& ARHS) const {
	return Active() && (FPID == ARHS.FPID);
}