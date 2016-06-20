//! @file CYBProcess.cpp Shared function implementations for CYB::Platform::Process
#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(Process&& AProcess) noexcept {
	AProcess.Terminate();
}

bool CYB::Platform::System::Process::operator!=(const Process& ARHS) const noexcept {
	return !(*this == ARHS);
}