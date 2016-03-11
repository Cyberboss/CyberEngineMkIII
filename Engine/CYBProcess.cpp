//! @file CYBProcess.cpp Shared function implementations for CYB::Platform::Process
#include "CYB.hpp"

void CYB::Platform::Process::Terminate(Process&& AProcess) {
	AProcess.Terminate();
}