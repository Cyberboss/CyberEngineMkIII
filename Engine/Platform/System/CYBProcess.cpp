//! @file CYBProcess.cpp Shared function implementations for CYB::Platform::Process
#include "CYB.hpp"

void CYB::Platform::System::Process::Terminate(Process&& AProcess) noexcept {
	AProcess.Terminate();
}