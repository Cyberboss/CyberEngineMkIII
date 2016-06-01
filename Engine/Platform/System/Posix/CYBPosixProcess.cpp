#include "CYB.hpp"

void CYB::Platform::Process::Terminate(void) {
	if (FPID == Posix::getpid())
		Sys::Call(Sys::EXIT, 0);
	Core().FModuleManager.FC.Call<Modules::LibC::kill>(FPID, SIGKILL);
}

CYB::Platform::Process CYB::Platform::Process::GetSelf(void) {
	return Process(Posix::getpid());
}

CYB::Platform::Implementation::Process::Process(CYB::Platform::Posix::pid_t APID) :
	FPID(APID)
{}

CYB::Platform::Implementation::Process::Process(Process&& AMove) :
	FPID(AMove.FPID)
{}
