//! @file CYBWin32Process.cpp Implements CYB::Platform::Process for Posix

#include "CYB.hpp"

void CYB::Platform::Process::Terminate(void) {

}

CYB::Platform::Process CYB::Platform::Process::GetSelf(void) {
	return Process();
}