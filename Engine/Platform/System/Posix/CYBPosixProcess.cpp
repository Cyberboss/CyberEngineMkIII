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

CYB::Platform::System::Implementation::Process::Process(Process&& AMove) :
	FPID(AMove.FPID)
{}
