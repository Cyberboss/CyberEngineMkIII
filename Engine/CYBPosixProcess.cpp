//! @file CYBWin32Process.cpp Implements CYB::Platform::Process for Posix

#include "CYB.hpp"

void CYB::Platform::Process::Terminate(void) {
	Implementation::Posix::kill(FPID, SIGKILL);
}

CYB::Platform::Process CYB::Platform::Process::GetSelf(void) {
	return Process(Implementation::Posix::getpid());
}

CYB::Platform::Implementation::Process::Process(CYB::Platform::Implementation::Posix::pid_t APID) :
	FPID(APID)
{}

CYB::Platform::Implementation::Process::Process(Process&& AMove) :
	FPID(AMove.FPID)
{}
